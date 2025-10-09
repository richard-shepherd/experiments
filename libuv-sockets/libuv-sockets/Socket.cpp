#include "Socket.h"
#include "Logger.h"
#include "Utils.h"
using namespace MessagingMesh;

// Constructor.
// NOTE: The constructor is private. Use Socket::create() to create an instance.
Socket::Socket(uv_loop_t* pLoop, ICallback* pCallback) :
    m_pLoop(pLoop),
    m_pCallback(pCallback)
{
}

// Destructor.
Socket::~Socket()
{
    Logger::info("Closing socket: " + m_name);
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

// Connects a client socket to the IP address and port specified.
void Socket::connectIP(const std::string& ipAddress, int port)
{

}

// Called when a new client conection is received.
void Socket::onNewConnection(uv_stream_t* server, int status)
{

}
