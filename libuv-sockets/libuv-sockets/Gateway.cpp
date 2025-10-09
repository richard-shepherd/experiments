#include "Gateway.h"
#include "Socket.h"
#include "Utils.h"
#include "Logger.h"
using namespace MessagingMesh;

// Constructor.
Gateway::Gateway(int port) :
    m_port(port),
    m_loopThread("GATEWAY")
{
    // We create a socket to listen to client connections, managed by the uv loop...
    m_loopThread.marshallEvent(
        [this](uv_loop_t* pLoop)
        {
            m_listeningSocket = Socket::create(pLoop, this);
            m_listeningSocket->listen(m_port);
        });
}

// Destructor.
Gateway::~Gateway()
{
}

// Socket::ICallback implementation.
// Called when data has been received on the socket.
void Gateway::onDataReceived(const NetworkData& networkData)
{
}

// Socket::ICallback implementation.
// Called when a new client connection has been made to a listening socket.
void Gateway::onNewConnection(const SocketPtr& clientSocket)
{
}

