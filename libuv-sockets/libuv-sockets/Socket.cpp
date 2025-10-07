#include "Socket.h"
using namespace MessagingMesh;

// Constructor.
// NOTE: The constructor is private. Use Socket::create() to create an instance.
Socket::Socket(uv_loop_t* pLoop) :
    m_pLoop(pLoop)
{
}

// Destructor.
Socket::~Socket()
{
}

// Connects a client socket to the IP address and port specified.
void Socket::connectIP(const std::string& ipAddress, int port)
{

}

