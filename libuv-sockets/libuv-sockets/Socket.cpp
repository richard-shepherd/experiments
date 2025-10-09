#include "Socket.h"
#include "Logger.h"
#include "Utils.h"
using namespace MessagingMesh;

// Constructor.
// NOTE: The constructor is private. Use Socket::create() to create an instance.
Socket::Socket(uv_loop_t* pLoop, ICallback* pCallback) :
    m_pLoop(pLoop),
    m_pCallback(pCallback)
{
}

// Destructor.
Socket::~Socket()
{
    Logger::info("Closing socket: " + m_name);
}

// Connects a server socket to listen on the specified port.
void Socket::listen(int port)
{
    m_name = Utils::format("LISTENING-SOCKET:%d", port);
    Logger::info("Creating socket: " + m_name);
}

// Connects a client socket to the IP address and port specified.
void Socket::connectIP(const std::string& ipAddress, int port)
{

}

