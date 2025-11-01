#pragma once
#include <map>
#include <string>
#include "SharedPointers.h"
#include "Socket.h"

namespace MessagingMesh
{
    // Forward declarations...
    class NetworkMessageHeader;

    /// <summary>
    /// Manages a messaging-mesh service. 
    /// 
    /// A service is a 'namespace' of messaging. Clients connecting to a service
    /// can send messages to each other, with message sent subjects only matching
    /// to subscriptions to the same service.
    /// 
    /// UV loop and thread
    /// ------------------
    /// Each service runs it own UV loop, so message processing for each service is
    /// managed on its own thread. As all updates on the UV loop take place on the
    /// (single) UV loop thread, this means that we do not have to lock service
    /// specific code such as the subject-matching engine.
    /// </summary>
    class ServiceManager : public Socket::ICallback
    {
    // Public methods...
    public:
        // Constructor.
        ServiceManager(const std::string& serviceName);

        // Destructor.
        ~ServiceManager() = default;

        // Registers a client socket to be managed for this service.
        void registerSocket(SocketPtr pSocket);

    // Socket::ICallback implementation...
    private:
        // Called when a new client connection has been made to a listening socket.
        // Called on the GATEWAY thread.
        void onNewConnection(SocketPtr /*pClientSocket*/) {}

        // Called when data has been received on the socket.
        // Called on the thread of the client socket.
        void onDataReceived(Socket* pSocket, BufferPtr pBuffer);

        // Called when a socket has been disconnected.
        // Called on the socket's thread.
        void onDisconnected(Socket* pSocket);

        // Called when the movement of the socket to a new UV loop has been completed.
        void onMoveToLoopComplete(Socket* pSocket);

    // Private functions...
    private:
        // Called when we receive a message.
        void onMessage(const Socket* pSocket, const NetworkMessageHeader& header, BufferPtr pBuffer);

    // Private data...
    private:
        // The service name...
        std::string m_serviceName;

        // UV loop for processing client messages...
        UVLoopPtr m_pUVLoop;

        // Client sockets, keyed by socket name...
        std::map<std::string, SocketPtr> m_clientSockets;
    };
} // namespace

