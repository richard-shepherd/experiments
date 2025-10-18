#include "Socket.h"
#include "Logger.h"
#include "Utils.h"
#include "UVUtils.h"
#include "UVLoop.h"
#include "NetworkData.h"
#include "OSSocketHolder.h"
using namespace MessagingMesh;

// Constructor.
// NOTE: The constructor is private. Use Socket::create() to create an instance.
Socket::Socket(UVLoop& uvLoop) :
    m_uvLoop(uvLoop),
    m_connected(false),
    m_pCallback(nullptr),
    m_pSocket(nullptr),
    m_pCurrentMessage(nullptr)
{
}

// Destructor.
Socket::~Socket()
{
    Logger::info("Closing socket: " + m_name);
    if (m_pSocket)
    {
        uv_close(
            (uv_handle_t*)m_pSocket,
            [](uv_handle_t* pHandle)
            {
                auto pSocket = (uv_tcp_t*)pHandle;
                delete pSocket;
            }
        );
    }
}

// Creates the UV socket.
// Note: This is not done in the constructor, as that can be called outside
//       the UV loop. It is only called from functions inside the loop.
void Socket::createSocket()
{
    // We create the socket and associate it with the loop.
    // We set its data to point to 'this' so that callbacks can invoke class methods.
    m_pSocket = new uv_tcp_t;
    m_pSocket->data = this;
    uv_tcp_init(
        m_uvLoop.getUVLoop(),
        m_pSocket
    );
}

// Sets the callback.
void Socket::setCallback(ICallback* pCallback)
{
    m_pCallback = pCallback;
}

// Called when a socket is connected to set up reading and writing.
void Socket::onSocketConnected()
{
    // We disable Nagling...
    uv_tcp_nodelay(m_pSocket, 1);

    // We note the the socket is connected and process any queued writes...
    m_connected = true;
    processQueuedWrites();

    // We start reading data from the socket...
    uv_read_start(
        (uv_stream_t*)m_pSocket,
        UVUtils::allocateBufferMemory,
        [](uv_stream_t* s, ssize_t n, const uv_buf_t* b)
        {
            auto self = (Socket*)s->data;
            self->onDataReceived(s, n, b);
        }
    );
}

// Connects a server socket to listen on the specified port.
void Socket::listen(int port)
{
    // We create a name for the socket from its connection info...
    m_name = Utils::format("LISTENING-SOCKET:%d", port);
    Logger::info("Creating socket: " + m_name);

    // We create the UV socket...
    createSocket();

    // We bind to the specified port on all network interfaces...
    struct sockaddr_in addr;
    uv_ip4_addr("0.0.0.0", port, &addr);
    uv_tcp_bind(m_pSocket, (const struct sockaddr*)&addr, 0);

    // We turn of Nagling...
    uv_tcp_nodelay(m_pSocket, 1);

    // We listen for connections, calling onNewConnection() when a connection is received...
    int listenResult = uv_listen(
        (uv_stream_t*)m_pSocket,
        MAX_INCOMING_CONNECTION_BACKLOG,
        [](uv_stream_t* p, int s)
        {
            auto self = (Socket*)p->data;
            self->onNewConnection(p, s);
        }
    );
    if (listenResult)
    {
        Logger::error(Utils::format("uv_listen error: %s", uv_strerror(listenResult)));
    }
}

// Connects the socket by accepting a listen request received by the server.
void Socket::accept(uv_stream_t* pServer)
{
    // We create the UV socket...
    createSocket();

    // We accept the connection...
    if (uv_accept(pServer, (uv_stream_t*)m_pSocket) == 0)
    {
        // We find the name of the client...
        auto peerInfo = UVUtils::getPeerIPInfo(m_pSocket);
        m_name = Utils::format("CLIENT-SOCKET:%s:%s", peerInfo.Hostname, peerInfo.Service);
        Logger::info("Accepted socket: " + m_name);

        // We start reading and writing...
        onSocketConnected();
    }
    else
    {
        Logger::error("Accept error");
        // RSSTODO: Should we close the socket here? Or other notification?
    }
}

// Connects a client socket to the IP address and port specified.
void Socket::connectIP(const std::string& ipAddress, int port)
{
    Logger::info(Utils::format("Connecting to %s:%d", ipAddress.c_str(), port));

    // We create the UV socket...
    createSocket();

    // We make the connection request...
    struct sockaddr_in destination;
    uv_ip4_addr(ipAddress.c_str(), port, &destination);
    auto pConnect = new uv_connect_t;
    pConnect->data = this;
    uv_tcp_connect(
        pConnect, 
        m_pSocket,
        (const struct sockaddr*)&destination, 
        [](uv_connect_t* r, int s)
        {
            auto self = (Socket*)r->data;
            self->onConnectCompleted(r, s);
        }
    );
}

// Connects to the socket specified.
void Socket::connectSocket(uv_os_sock_t socket)
{
    Logger::info(Utils::format("Connecting to specified socket %d", socket));

    // We create the UV socket...
    createSocket();

    // We open the socket, connecting to the socket passed in...
    auto status = uv_tcp_open(m_pSocket, socket);
    if (status != 0)
    {
        Logger::error(Utils::format("uv_tcp_open failed: %s", uv_strerror(status)));
        return;
    }

    // We start reading and writing...
    onSocketConnected();
}

// Called at the client side when a client has connected to a server.
void Socket::onConnectCompleted(uv_connect_t* pRequest, int status)
{
    try
    {
        if (status < 0)
        {
            Logger::error(Utils::format("Connection error: %s", uv_strerror(status)));
            return;
        }

        // We start reading and writing...
        onSocketConnected();
    }
    catch (const std::exception& ex)
    {
        Logger::error(Utils::format("%s: %s", __func__, ex.what()));
    }
}

// Writes data to the socket.
void Socket::write(NetworkDataPtr pNetworkData)
{
    auto pWriteRequest = UVUtils::allocateWriteRequest(pNetworkData);
    pWriteRequest->write_request.data = this;
    uv_write(
        &pWriteRequest->write_request, 
        (uv_stream_t*)m_pSocket,
        &pWriteRequest->buffer, 
        1, 
        [](uv_write_t* r, int s)
        {
            auto self = (Socket*)r->data;
            self->onWriteCompleted(r, s);
        }
    );
}

// Called when a write request has completed.
void Socket::onWriteCompleted(uv_write_t* pRequest, int status)
{
    try
    {
        // We check the status...
        if (status < 0)
        {
            Logger::error(Utils::format("Write error: %s", uv_strerror(status)));
        }

        // We release the write request (including the buffer)...
        auto pWriteRequest = (UVUtils::WriteRequest*)pRequest;
        UVUtils::releaseWriteRequest(pWriteRequest);
    }
    catch (const std::exception& ex)
    {
        Logger::error(Utils::format("%s: %s", __func__, ex.what()));
    }
}

// Queues data to be written to the socket.
// Can be called from any thread, not just from the uv loop thread.
// Queued writes will be coalesced into one network update.
// RSSTODO: We need some way to slow down the client if it publishes too much too fast.
void Socket::queueWrite(NetworkDataPtr pNetworkData)
{
    // We queue the data to write...
    m_queuedWrites.add(pNetworkData);

    // We marshall an event to write the data. As this does not take place straight 
    // away, this allows us to coalesce multiple queued writes...
    m_uvLoop.marshallUniqueEvent(
        UVLoop::UniqueEventKey::SOCKET_QUEUE_WRITE,
        [this](uv_loop_t* pLoop)
        {
            processQueuedWrites();
        }
    );
}

// Detaches the socket and returns a copy (dup) of it.
OSSocketHolderPtr Socket::detachSocket()
{
    // We duplicate the socket...
    auto duplicateSocket = UVUtils::duplicateSocket(m_pSocket->socket);

    // Note: We do not close the original socket here. It will be closed when
    //       this socket object is destructed.

    // We return the socket in an OSSocketHolder. This helps with
    // its visibility across threads...
    auto pSocketHolder = OSSocketHolder::create();
    pSocketHolder->setSocket(duplicateSocket);
    return pSocketHolder;
}

// Sends a coalesced network message for all queued writes.
void Socket::processQueuedWrites()
{
    try
    {
        // We check if the socket is connected...
        if (!m_connected)
        {
            return;
        }

        // We find the combined size of the queued writes...
        auto queuedWrites = m_queuedWrites.getItems();
        size_t totalSize = 0;
        for (auto queuedWrite : *queuedWrites)
        {
            totalSize += queuedWrite->getDataSize();
        }

        // We create a write-request with a buffer to hold all the queued items...
        auto pWriteRequest = UVUtils::allocateWriteRequest(totalSize);
        pWriteRequest->write_request.data = this;

        // We copy the data into the buffer...
        size_t bufferPosition = 0;
        for (auto queuedWrite : *queuedWrites)
        {
            auto itemSize = queuedWrite->getDataSize();
            auto itemData = queuedWrite->getData();
            memcpy(pWriteRequest->buffer.base + bufferPosition, itemData, itemSize);
            bufferPosition += itemSize;
        }

        // We write the combined buffer...
        uv_write(
            &pWriteRequest->write_request,
            (uv_stream_t*)m_pSocket,
            &pWriteRequest->buffer,
            1,
            [](uv_write_t* r, int s)
            {
                auto self = (Socket*)r->data;
                self->onWriteCompleted(r, s);
            }
        );
    }
    catch (const std::exception& ex)
    {
        Logger::error(Utils::format("%s: %s", __func__, ex.what()));
    }
}

// Called at the server side when a new client connection has been received
// on a listening socket.
void Socket::onNewConnection(uv_stream_t* pServer, int status)
{
    try
    {
        // We check that the connection has succeeded...
        Logger::info("onNewConnection");
        if (status < 0)
        {
            Logger::error(Utils::format("Connection error: %s", uv_strerror(status)));
            return;
        }

        // We create a client socket for the new connection...
        auto clientSocket = Socket::create(m_uvLoop);
        clientSocket->accept(pServer);

        // We pass the socket to the callback...
        if (m_pCallback) m_pCallback->onNewConnection(clientSocket);
    }
    catch (const std::exception& ex)
    {
        Logger::error(Utils::format("%s: %s", __func__, ex.what()));
    }
}

// Called when data has been received on a socket.
void Socket::onDataReceived(uv_stream_t* pStream, ssize_t nread, const uv_buf_t* pBuffer)
{
    try
    {
        // All data we receive comes in the form of network-data messages.
        // These look like:
        // - size  (int32, little-endian)
        // - bytes[size]
        //
        // There are different cases where we can get this callback:
        // 
        // 1. A data buffer containing all (and only) the data for one message.
        //
        // 2. A data buffer containing multiple messages.
        // 
        // 3. Partial data for a message. In particular this will be the case
        //    for very large messages which do not fit into one data buffer.
        //
        // It could be that the buffer holds data for some combination of the
        // types described above. For example:
        // - Two complete messages followed by one partial message.
        // - The remainder of a message from a previous update, plus one or more new messages.
        // - Other permutations and combinations along these lines.
        // 
        // In all cases we only really need to know two things:
        // 
        // a) Are we expecting a new message? 
        //    If so, we read the size and then read that number of bytes.
        // 
        // b) Are we in the process of reading a message?
        //    We have a message that started in a previous update and has data
        //    continuing into this one.
        // 
        // We are always reading one message at a time into the m_currentMessage object.
        // This is a shared pointer to a NetworkData object.
        // - If this is null we are expecting a new message and need to read the size. 
        // - If this is not null we are reading additional data for an existing message.
        // 
        // The NetworkData object knows the size we are expecting, so we can continue to
        // read data until we have all the data we need for the message.
        // 
        // NOTE: We need to be careful when reading the size for a new message. It is
        //       possible that the size itself may only be received across multiple of
        //       these callbacks.

        // We read the buffer...
        size_t bufferSize = nread;
        size_t bufferPosition = 0;
        while (bufferPosition < bufferSize)
        {
            // If we do not have a current message we create one...
            if (!m_pCurrentMessage)
            {
                m_pCurrentMessage = NetworkData::create();
            }

            // We read data into the current message...
            size_t bytesRead = m_pCurrentMessage->read(pBuffer->base, bufferSize, bufferPosition);

            // If we have read all data for the current message we call back with it.
            // We can then clear the message to start a new one.
            if (m_pCurrentMessage->hasAllData())
            {
                if (m_pCallback) m_pCallback->onDataReceived(m_pCurrentMessage);
                m_pCurrentMessage = nullptr;
            }

            // We update the buffer position and loop to check if there is
            // more data to read...
            bufferPosition += bytesRead;
        }

        // We release the buffer memory...
        UVUtils::releaseBufferMemory(pBuffer);
    }
    catch (const std::exception& ex)
    {
        Logger::error(Utils::format("%s: %s", __func__, ex.what()));
    }
}

