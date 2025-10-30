#include "Connection.h"
#include "ConnectionImpl.h"
using namespace MessagingMesh;

// Constructor.
Connection::Connection(const std::string& hostname, int port, const std::string& service) :
    m_pImpl(std::make_unique<ConnectionImpl>(hostname, port, service))
{
}

// Destructor.
Connection::~Connection() = default;

// Sends a message to the specified subject.
void Connection::sendMessage(const std::string& subject, const MessagePtr& pMessage)
{
    m_pImpl->sendMessage(subject, pMessage);
}

