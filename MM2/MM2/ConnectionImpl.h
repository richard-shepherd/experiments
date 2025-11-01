#pragma once
#include <string>
#include "SharedPointers.h"
#include "Socket.h"
#include "AutoResetEvent.h"

namespace MessagingMesh
{
    // Forward declarations...
    class NetworkMessage;

    /// <summary>
    /// Implementation of the Connection class, ie a client connection
    /// to the messaging-mesh.
    /// </summary>
    class ConnectionImpl : Socket::ICallback
    {
    // Public methods...
    public:
        // Constructor.
        ConnectionImpl(const std::string& hostname, int port, const std::string& service);

        // Destructor.
        ~ConnectionImpl();

        // Sends a message to the specified subject.
        void sendMessage(const std::string& subject, const MessagePtr& pMessage) const;

    // Socket::ICallback implementation
    private:
        // Called when a new client connection has been made to a listening socket.
        // Called on the UV loop thread.
        void onNewConnection(SocketPtr /*pClientSocket*/) {}

        // Called when data has been received on the socket.
        // Called on the UV loop thread.
        void onDataReceived(Socket* pSocket, BufferPtr pBuffer);

        // Called when a socket has been disconnected.
        void onDisconnected(Socket* pSocket);

        // Called when the movement of the socket to a new UV loop has been completed.
        void onMoveToLoopComplete(Socket* /*pSocket*/) {}

    // Private functions...
    private:
        // Called when we see the ACK message from the Gateway.
        void onAck();

    // Private data...
    private:
        // Construction params...
        std::string m_hostname;
        int m_port;
        std::string m_service;

        // UV loop for client messaging...
        UVLoopPtr m_pUVLoop;

        // Socket connection to the gateway...
        SocketPtr m_pSocket;

        // Waits for the ACK signal...
        AutoResetEvent m_ackSignal;
    };
} // namespace


