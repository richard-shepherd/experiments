#pragma once
#include <memory>

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
        Connection();
        
    // Private data...
    private:
        // Implementation...
        std::unique_ptr<ConnectionImpl> m_pImpl;
    };
} // namespace


