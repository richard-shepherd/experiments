#pragma once
#include <memory>
#include <string>
#include "SharedPointers.h"

namespace MessagingMesh
{
    // Forward declarations...
    class ConnectionImpl;

    /// <summary>
    /// Client connection to the messaging-mesh.
    /// </summary>
    class Connection
    {
    // Public methods...
    public:
        // Constructor.
        Connection(const std::string& hostname, int port, const std::string& service);

        // Destructor.
        ~Connection();

        // Sends a message to the specified subject.
        void sendMessage(const std::string& subject, const MessagePtr& pMessage);
        
    // Private data...
    private:
        // Implementation...
        std::unique_ptr<ConnectionImpl> m_pImpl;
    };
} // namespace


