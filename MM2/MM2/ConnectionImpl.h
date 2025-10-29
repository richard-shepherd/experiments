#pragma once
#include <string>
#include "SharedPointers.h"

namespace MessagingMesh
{
    // Forward declarations...
    class NetworkMessage;

    /// <summary>
    /// Implementation of the Connection class, ie a client connection
    /// to the messaging-mesh.
    /// </summary>
    class ConnectionImpl
    {
    // Public methods...
    public:
        // Constructor.
        ConnectionImpl(const std::string& hostname, int port, const std::string& service);

        // Destructor.
        ~ConnectionImpl();

        // Sends a message to the specified subject.
        void sendMessage(const std::string& subject, const MessagePtr& pMessage) const;

    // Private functions...
    private:
        // Sends a network-message to the gateway.
        void sendNetworkMessage(const NetworkMessage& networkMessage) const;

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
    };
} // namespace


