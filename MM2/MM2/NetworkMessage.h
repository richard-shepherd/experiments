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

        // Gets a reference to the header.
        NetworkMessageHeader& getHeader() { return m_header; }

        // Gets the message.
        MessagePtr getMessage() const;

        // Sets the message.
        void setMessage(MessagePtr pMessage);

        // Serializes the network message to the current position of the buffer.
        void serialize(Buffer& buffer) const;

        // Deserialized the network message from the current position in the buffer.
        void deserialize(Buffer& buffer);

    // Private functions...
    private:
        // Creates the message we hold if it does not already exist.
        void createMessageIfItDoesNotExist() const;
        
    // Private data...
    private:
        // Header...
        NetworkMessageHeader m_header;

        // Message payload...
        mutable MessagePtr m_pMessage = nullptr;
    };
} // namespace


