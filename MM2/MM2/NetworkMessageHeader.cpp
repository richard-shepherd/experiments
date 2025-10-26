#include "NetworkMessageHeader.h"
#include "Buffer.h"
using namespace MessagingMesh;

// Constructor.
NetworkMessageHeader::NetworkMessageHeader()
{
}

// Serializes the network message header to the current position of the buffer.
void NetworkMessageHeader::serialize(Buffer& buffer) const
{
    // Subscription ID...
    buffer.write(m_subscriptionID);

    // Subject...
    buffer.write(m_subject);

    // Reply subject...
    buffer.write(m_replySubject);

    // Action...
    buffer.write(static_cast<int8_t>(m_action));
}

// Deserialized the network message header from the current position in the buffer.
void NetworkMessageHeader::deserialize(Buffer& buffer)
{
    // Subscription ID...
    m_subscriptionID = buffer.readUInt32();

    // Subject...
    m_subject = buffer.readString();

    // Reply subject...
    m_replySubject = buffer.readString();

    // Action...
    m_action = static_cast<Action>(buffer.readInt8());
}
