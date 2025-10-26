#include "Connection.h"
#include "ConnectionImpl.h"
using namespace MessagingMesh;

// Constructor.
Connection::Connection(const std::string& hostname, int port, const std::string& service) :
    m_pImpl(std::make_unique<ConnectionImpl>(hostname, port, service))
{
}
