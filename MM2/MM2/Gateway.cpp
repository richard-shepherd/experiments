#include "Gateway.h"
#include "Socket.h"
#include "Utils.h"
#include "Logger.h"
#include "NetworkMessage.h"
#include "Message.h"
#include "Field.h"
using namespace MessagingMesh;

// Constructor.
Gateway::Gateway(int port) :
    m_port(port),
    m_pUVLoop(UVLoop::create("GATEWAY"))
{
    // We create a socket to listen to client connections, managed by the uv loop...
    m_pUVLoop->marshallEvent(
        [this](uv_loop_t* /*pLoop*/)
        {
            createListeningSocket();
        }
    );
}

// Creates the socket to listen for client connections.
void Gateway::createListeningSocket()
{
    try
    {
        m_listeningSocket = Socket::create(m_pUVLoop);
        m_listeningSocket->setCallback(this);
        m_listeningSocket->listen(m_port);
    }
    catch (const std::exception& ex)
    {
        Logger::error(Utils::format("%s: %s", __func__, ex.what()));
    }
}

// Called when a new client connection has been made to a listening socket.
// Called on the GATEWAY thread.
void Gateway::onNewConnection(SocketPtr pClientSocket)
{
    try
    {
        // We add the socket to the pending-collection and observe it 
        // to listen for the CONNECT message...
        m_pendingConnections[pClientSocket->getName()] = pClientSocket;
        pClientSocket->setCallback(this);

        //// We move the socket to the client loop...
        //pClientSocket->moveToLoop(m_pUVClientLoop);
    }
    catch (const std::exception& ex)
    {
        Logger::error(Utils::format("%s: %s", __func__, ex.what()));
    }
}

// Called when data has been received on the socket.
// Called on the thread of the client socket.
void Gateway::onDataReceived(const std::string& socketName, BufferPtr pBuffer)
{
    try
    {
        // The buffer holds a serialized NetworkMessage. We deserialize
        // the header and check the action.
        NetworkMessage networkMessage;
        networkMessage.deserializeHeader(*pBuffer);
        auto& header = networkMessage.getHeader();
        auto action = static_cast<int8_t>(header.getAction());
        Logger::info(Utils::format("Received: socket=%s, action=%d", socketName.c_str(), action));
    }
    catch (const std::exception& ex)
    {
        Logger::error(Utils::format("%s: %s", __func__, ex.what()));
    }
}

// Called when a socket has been disconnected.
// Called on the socket's thread.
void Gateway::onDisconnected(const std::string& /*socketName*/)
{
    try
    {
    }
    catch (const std::exception& ex)
    {
        Logger::error(Utils::format("%s: %s", __func__, ex.what()));
    }
}
