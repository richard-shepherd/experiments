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
            m_listeningSocket = Socket::create(pLoop);
            m_listeningSocket->setCallback(this);
            m_listeningSocket->listen(m_port);
        });
}

// Destructor.
Gateway::~Gateway()
{
}

// Called when data has been received on the socket.
// Called on the UV loop thread.
void Gateway::onDataReceived(NetworkDataPtr networkData)
{
}

// Called when a new client connection has been made to a listening socket.
// Called on the UV loop thread.
void Gateway::onNewConnection(SocketPtr clientSocket)
{
}

