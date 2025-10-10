#include "Socket.h"
#include "Logger.h"
#include "Utils.h"
#include "UVUtils.h"
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
    // TODO: clean up the socket (if it was created)
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
void Socket::accept(uv_stream_t* server)
{
    // We create the client socket...
    m_uvSocket = std::make_unique<uv_tcp_t>();
    uv_tcp_init(m_pLoop, m_uvSocket.get());

    // We accept the connection...
    if (uv_accept(server, (uv_stream_t*)m_uvSocket.get()) == 0)
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
            UVUtils::allocBuffer,
            [](uv_stream_t* s, ssize_t n, const uv_buf_t* b)
            {
                auto self = (Socket*)s->data;
                self->onDataReceived(s, n, b);
            });
    }
    else
    {
        Logger::error("Accept error");
        // TODO: Should we close the socket here? Or other notification?
    }
}

// Connects a client socket to the IP address and port specified.
void Socket::connectIP(const std::string& ipAddress, int port)
{

}

// Called when a new client conection is received.
void Socket::onNewConnection(uv_stream_t* server, int status)
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
        clientSocket->accept(server);

        // We pass the socket to the callback...
        if (m_pCallback)
        {
            m_pCallback->onNewConnection(clientSocket);
        }
    }
    catch (const std::exception& ex)
    {
        Logger::error(Utils::format("%s: %s", __func__, ex.what()));
    }
}

// Called when data has been received on a socket.
void Socket::onDataReceived(uv_stream_t* pClientStream, ssize_t nread, const uv_buf_t* pBuffer)
{
    try
    {
    }
    catch (const std::exception& ex)
    {
        Logger::error(Utils::format("%s: %s", __func__, ex.what()));
    }
}
