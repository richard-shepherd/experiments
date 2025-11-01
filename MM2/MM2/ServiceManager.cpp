#include "ServiceManager.h"
#include "UVLoop.h"
#include "Socket.h"
#include "Logger.h"
#include "Utils.h"
#include "NetworkMessage.h"
using namespace MessagingMesh;

// Constructor.
ServiceManager::ServiceManager(const std::string& serviceName) :
    m_serviceName(serviceName),
    m_pUVLoop(UVLoop::create(serviceName))
{
}

// Registers a client socket to be managed for this service.
void ServiceManager::registerSocket(SocketPtr pSocket)
{
    // We add the socket to the collection of active clients...
    m_clientSockets[pSocket->getName()] = pSocket;

    // We observe updates from the socket...
    pSocket->setCallback(this);

    // We move the socket to our UV loop...
    pSocket->moveToLoop(m_pUVLoop);
}

// Called when data has been received on the socket.
// Called on the thread of the client socket.
void ServiceManager::onDataReceived(Socket* pSocket, BufferPtr pBuffer)
{
    try
    {
        // The buffer holds a serialized NetworkMessage. We deserialize
        // the header and check the action.
        NetworkMessage networkMessage;
        networkMessage.deserializeHeader(*pBuffer);
        auto& header = networkMessage.getHeader();
        auto action = header.getAction();

        // RSSTODO: REMOVE THIS!!!
        Logger::info(Utils::format("ServiceManager::onDataReceived, action=%d", static_cast<int8_t>(action)));

        switch (action)
        {
        case NetworkMessageHeader::Action::SEND_MESSAGE:
            onMessage(pSocket, header, pBuffer);
            break;
        }
    }
    catch (const std::exception& ex)
    {
        Logger::error(Utils::format("%s: %s", __func__, ex.what()));
    }
}

// Called when a socket has been disconnected.
// Called on the socket's thread.
void ServiceManager::onDisconnected(Socket* pSocket)
{
    try
    {
        // We remove the socket from the collection of client sockets...
        auto& socketName = pSocket->getName();
        m_clientSockets.erase(socketName);
    }
    catch (const std::exception& ex)
    {
        Logger::error(Utils::format("%s: %s", __func__, ex.what()));
    }
}

// Called when the movement of the socket to a new UV loop has been completed.
void ServiceManager::onMoveToLoopComplete(Socket* pSocket)
{
    try
    {
        // We send an ACK message to the client to let them know that the
        // CONNECT has completed successfully...
        NetworkMessage connectMessage;
        auto& header = connectMessage.getHeader();
        header.setAction(NetworkMessageHeader::Action::ACK);
        Utils::sendNetworkMessage(connectMessage, pSocket);
    }
    catch (const std::exception& ex)
    {
        Logger::error(Utils::format("%s: %s", __func__, ex.what()));
    }
}

// Called when we receive a message.
void ServiceManager::onMessage(const Socket* /*pSocket*/, const NetworkMessageHeader& /*header*/, BufferPtr /*pBuffer*/)
{
}


