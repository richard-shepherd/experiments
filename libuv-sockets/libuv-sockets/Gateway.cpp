#include "Gateway.h"
#include "Socket.h"
#include "Utils.h"
#include "Logger.h"
using namespace MessagingMesh;

// Constructor.
Gateway::Gateway(int port) :
    m_port(port),
    m_uvLoop("GATEWAY")
{
    // We create a socket to listen to client connections, managed by the uv loop...
    m_uvLoop.marshallEvent(
        [this](uv_loop_t* pLoop)
        {
            // RSSTODO: Needs try-catch on this callback
            m_listeningSocket = Socket::create(m_uvLoop);
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
    static int count = 0;

    count++;
    if (count % 100000 == 0)
    {
        Logger::info("Received data!!!");
    }
}

// Called when a new client connection has been made to a listening socket.
// Called on the UV loop thread.
void Gateway::onNewConnection(SocketPtr clientSocket)
{
    clientSocket->setCallback(this);
    m_clientSockets.push_back(clientSocket);
}

