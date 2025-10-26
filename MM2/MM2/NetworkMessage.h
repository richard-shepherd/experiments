#pragma once
#include "NetworkMessageHeader.h"
#include "SharedPointers.h"

namespace MessagingMesh
{
    /// <summary>
    /// Message sent between messaging-mesh clients and gateways for
    /// updates and events. Includes a header indicating the type of
    /// update being sent, plus a message payload.
    /// </summary>
    class NetworkMessage
    {
    // Public methods...
    public:
        // Constructor.
        NetworkMessage();

        // Serializes the network message to the current position of the buffer.
        void serialize(Buffer& buffer) const;

        // Deserialized the network message from the current position in the buffer.
        void deserialize(Buffer& buffer);
        
    // Private data...
    private:
        // Header...
        NetworkMessageHeader m_header;

        // Message payload...
        MessagePtr m_pMessage;
    };
} // namespace


