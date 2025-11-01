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
void ServiceManager::onDataReceived(const Socket* pSocket, BufferPtr pBuffer)
{
    try
    {
        // TODO: REMOVE THIS!!! It might lead to out-of-order messaging.
        // It is possible that we received data from the Gateway's UV loop.
        // This can happen for messages sent by the client before the socket has
        // been moved to the loop for the service. We want to process all messages
        // on our own UV loop and thread, so if we see messages from another loop
        // we marshall them to our loop...
        if (!pSocket->isSameUVLoop(m_pUVLoop))
        {
            static int count = 0;
            Logger::info(Utils::format("TODO: REMOVE THIS. Not same loop: %d", ++count));
            m_pUVLoop->marshallEvent(
                [this, pSocket, pBuffer](uv_loop_t* /*pLoop*/)
                {
                    onDataReceived(pSocket, pBuffer);
                }
            );
            return;
        }

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
void ServiceManager::onDisconnected(const Socket* pSocket)
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

// Called when we receive a message.
void ServiceManager::onMessage(const Socket* /*pSocket*/, const NetworkMessageHeader& /*header*/, BufferPtr /*pBuffer*/)
{
}


