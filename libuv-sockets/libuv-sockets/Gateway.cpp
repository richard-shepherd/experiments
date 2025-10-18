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
        [this](uv_loop_t* pLoop)
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
// Called on the UV loop thread.
void Gateway::onDataReceived(NetworkDataPtr pNetworkData)
{
    static int count = 0;

    count++;
    if (count % 1000000 == 0)
    {
        Logger::info(Utils::format("Received data: %d", count));
    }
}

// Called when a new client connection has been made to a listening socket.
// Called on the UV loop thread.
void Gateway::onNewConnection(SocketPtr pClientSocket)
{
    // We duplicate the socket and run it on the client loop...
    auto pSocketHolder = pClientSocket->detachSocket();
    auto pMovedSocket = Socket::create(m_pUVClientLoop);
    m_pUVClientLoop->marshallEvent(
        [pSocketHolder, pMovedSocket](uv_loop_t*)
        {
            auto socket = pSocketHolder->getSocket();
            pMovedSocket->connectSocket(socket);
        }
    );
    pMovedSocket->setCallback(this);
    m_clientSockets.push_back(pMovedSocket);
}

