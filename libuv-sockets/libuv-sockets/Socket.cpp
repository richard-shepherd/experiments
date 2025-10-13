#include "Socket.h"
#include "Logger.h"
#include "Utils.h"
#include "UVUtils.h"
#include "NetworkData.h"
using namespace MessagingMesh;

// Constructor.
// NOTE: The constructor is private. Use Socket::create() to create an instance.
Socket::Socket(uv_loop_t* pLoop) :
    m_pLoop(pLoop),
    m_pCallback(nullptr)
{
}

// Destructor.
Socket::~Socket()
{
    Logger::info("Closing socket: " + m_name);
    // RSSTODO: clean up the socket (if it was created)
}

// Sets the callback.
void Socket::setCallback(ICallback* pCallback)
{
    m_pCallback = pCallback;
}

// Connects a server socket to listen on the specified port.
void Socket::listen(int port)
{
    // We create a name for the socket from its connection info...
    m_name = Utils::format("LISTENING-SOCKET:%d", port);
    Logger::info("Creating socket: " + m_name);

    // We create a socket to listen for incoming connections...
    m_uvSocket = std::make_unique<uv_tcp_t>();
    uv_tcp_init(m_pLoop, m_uvSocket.get());

    // We bind to the specified port on all network interfaces...
    struct sockaddr_in addr;
    uv_ip4_addr("0.0.0.0", port, &addr);
    uv_tcp_bind(m_uvSocket.get(), (const struct sockaddr*)&addr, 0);

    // We turn of Nagling...
    uv_tcp_nodelay(m_uvSocket.get(), 1);

    // We listen for connections, calling onNewConnection() when a connection is received...
    m_uvSocket->data = this;
    int listenResult = uv_listen(
        (uv_stream_t*)m_uvSocket.get(),
        MAX_INCOMING_CONNECTION_BACKLOG,
        [](uv_stream_t* p, int s)
        {
            auto self = (Socket*)p->data;
            self->onNewConnection(p, s);
        });
    if (listenResult)
    {
        Logger::error(Utils::format("uv_listen error: %s", uv_strerror(listenResult)));
    }
}

// Connects the socket by accepting a listen request received by the server.
void Socket::accept(uv_stream_t* pServer)
{
    // We create the client socket...
    m_uvSocket = std::make_unique<uv_tcp_t>();
    uv_tcp_init(m_pLoop, m_uvSocket.get());

    // We accept the connection...
    if (uv_accept(pServer, (uv_stream_t*)m_uvSocket.get()) == 0)
    {
        // We find the name of the client...
        auto peerInfo = UVUtils::getPeerIPInfo(m_uvSocket.get());
        m_name = Utils::format("CLIENT-SOCKET:%s:%s", peerInfo.Hostname, peerInfo.Service);
        Logger::info("Accepted socket: " + m_name);

        // We disable Nagling...
        uv_tcp_nodelay(m_uvSocket.get(), 1);

        // We start reading data from the socket...
        m_uvSocket->data = this;
        uv_read_start(
            (uv_stream_t*)m_uvSocket.get(),
            UVUtils::allocateBufferMemory,
            [](uv_stream_t* s, ssize_t n, const uv_buf_t* b)
            {
                auto self = (Socket*)s->data;
                self->onDataReceived(s, n, b);
            });
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

    // We create a socket to listen for incoming connections...
    m_uvSocket = std::make_unique<uv_tcp_t>();
    uv_tcp_init(m_pLoop, m_uvSocket.get());

    // We make the connection request...
    struct sockaddr_in destination;
    uv_ip4_addr(ipAddress.c_str(), port, &destination);
    auto pConnect = new uv_connect_t;
    pConnect->data = this;
    uv_tcp_connect(
        pConnect, 
        m_uvSocket.get(), 
        (const struct sockaddr*)&destination, 
        [](uv_connect_t* r, int s)
        {
            auto self = (Socket*)r->data;
            self->onConnectCompleted(r, s);
        });
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

        // We disable Nagling...
        uv_tcp_nodelay((uv_tcp_t*)m_uvSocket.get(), 1);

        // We listen for data sent to us from the server...
        m_uvSocket->data = this;
        uv_read_start(
            (uv_stream_t*)m_uvSocket.get(),
            UVUtils::allocateBufferMemory,
            [](uv_stream_t* s, ssize_t n, const uv_buf_t* b)
            {
                auto self = (Socket*)s->data;
                self->onDataReceived(s, n, b);
            });
    }
    catch (const std::exception& ex)
    {
        Logger::error(Utils::format("%s: %s", __func__, ex.what()));
    }
}

// Writes data to the socket.
void Socket::write(const NetworkDataPtr& pNetworkData)
{
    auto pWriteRequest = UVUtils::allocateWriteRequest(pNetworkData);
    pWriteRequest->write_request.data = this;
    uv_write(
        &pWriteRequest->write_request, 
        (uv_stream_t*)m_uvSocket.get(),
        &pWriteRequest->buffer, 
        1, 
        [](uv_write_t* r, int s)
        {
            auto self = (Socket*)r->data;
            self->onWriteCompleted(r, s);
        });
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

// Called at the server side when a new client conection is received.
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
        auto clientSocket = Socket::create(m_pLoop);
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
            if (!m_currentMessage)
            {
                m_currentMessage = NetworkData::create();
            }

            // We read data into the current message...
            size_t bytesRead = m_currentMessage->read(pBuffer->base, bufferSize, bufferPosition);

            // If we have read all data for the current message we call back with it.
            // We can then clear the message to start a new one.
            if (m_currentMessage->hasAllData())
            {
                if (m_pCallback) m_pCallback->onDataReceived(m_currentMessage);
                m_currentMessage = nullptr;
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

