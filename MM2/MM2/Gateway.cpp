#include "Gateway.h"
#include "Socket.h"
#include "Utils.h"
#include "Logger.h"
#include "OSSocketHolder.h"
using namespace MessagingMesh;

// Constructor.
Gateway::Gateway(int port) :
    m_port(port),
    m_pUVLoop(UVLoop::create("GATEWAY")),
    m_pUVClientLoop(UVLoop::create("CLIENT-LOOP"))
{
    // We create a socket to listen to client connections, managed by the uv loop...
    m_pUVLoop->marshallEvent(
        [this](uv_loop_t* /*pLoop*/)
        {
            // RSSTODO: Needs try-catch on this callback
            m_listeningSocket = Socket::create(m_pUVLoop);
            m_listeningSocket->setCallback(this);
            m_listeningSocket->listen(m_port);
        }
    );
}

// Destructor.
Gateway::~Gateway()
{
}

// Called when data has been received on the socket.
// Called on the thread of the client socket.
void Gateway::onDataReceived(NetworkDataPtr /*pNetworkData*/)
{
    static int count = 0;

    count++;
    //Logger::info(Utils::format("Received data: %d", count));
    if (count % 1000000 == 0)
    {
        Logger::info(Utils::format("Received data: %d", count));
    }
}

// Called when a new client connection has been made to a listening socket.
// Called on the GATEWAY thread.
void Gateway::onNewConnection(SocketPtr pClientSocket)
{
    // We observe the socket...
    pClientSocket->setCallback(this);
    m_clientSockets[pClientSocket->getName()] = pClientSocket;

    // We move the socket to the client loop...
    pClientSocket->moveToLoop(m_pUVClientLoop);
}

// Called when a socket has been disconnected.
        // Called when a socket has been disconnected.
        // Called on the socket's thread.
void Gateway::onDisconnected(const std::string& socketName)
{
    m_clientSockets.erase(socketName);
}
