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
    m_pUVLoop(UVLoop::create("GATEWAY")),
    m_pUVClientLoop(UVLoop::create("CLIENT-LOOP"))
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

// Called when data has been received on the socket.
// Called on the thread of the client socket.
void Gateway::onDataReceived(BufferPtr pBuffer)
{
    try
    {
        // We deserialize the buffer to a NetworkMessage...
        NetworkMessage networkMessage;
        networkMessage.deserialize(*pBuffer);
        auto& header = networkMessage.getHeader();
        auto action = header.getAction();
        if (action == NetworkMessageHeader::Action::SEND_MESSAGE)
        {
            auto& subject = header.getSubject();
            auto pMessage = networkMessage.getMessage();
            auto value = pMessage->getField("VALUE")->getSignedInt32();
            Logger::info(Utils::format("Received: subject=%s, value=%d", subject.c_str(), value));
        }
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
        // We observe the socket...
        pClientSocket->setCallback(this);
        m_clientSockets[pClientSocket->getName()] = pClientSocket;

        // We move the socket to the client loop...
        pClientSocket->moveToLoop(m_pUVClientLoop);
    }
    catch (const std::exception& ex)
    {
        Logger::error(Utils::format("%s: %s", __func__, ex.what()));
    }
}

// Called when a socket has been disconnected.
// Called on the socket's thread.
void Gateway::onDisconnected(const std::string& socketName)
{
    try
    {
        m_clientSockets.erase(socketName);
    }
    catch (const std::exception& ex)
    {
        Logger::error(Utils::format("%s: %s", __func__, ex.what()));
    }
}
