#include "Sockets.h"
using namespace MessagingMesh;

// Static members...
std::atomic<int> Sockets::m_initializationCount{ 0 };

// Initializes sockets (for example the winsock API).
void Sockets::initialize()
{
    // We check if sockets have already been initialized...
    auto initializationCount = m_initializationCount++;
    if (initializationCount > 1)
    {
        // Sockets has already been initialized...
        return;
    }

    // 
}

// Cleans up sockets.
void Sockets:: cleanup()
{

}
