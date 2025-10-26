#include "NetworkMessage.h"
#include "Message.h"
using namespace MessagingMesh;

// Constructor.
NetworkMessage::NetworkMessage() :
    m_pMessage(Message::create())
{
}

// Serializes the network message to the current position of the buffer.
void NetworkMessage::serialize(Buffer& buffer) const
{
    // Header...
    m_header.serialize(buffer);

    // Message...
    m_pMessage->serialize(buffer);
}

// Deserialized the network message from the current position in the buffer.
void NetworkMessage::deserialize(Buffer& buffer)
{
    // Header...
    m_header.deserialize(buffer);

    // Message...
    m_pMessage->deserialize(buffer);
}
