#include "Connection.h"
#include "ConnectionImpl.h"
using namespace MessagingMesh;

// Constructor.
Connection::Connection() :
    m_pImpl(std::make_unique<ConnectionImpl>())
{
}
