#include <winsock2.h>
#include "Logger.h"
#include "Sockets.h"
#include "Utils.h"
using namespace MessagingMesh;

// Static members...
std::mutex Sockets::m_initializationMutex;
int Sockets::m_initializationCount = 0;
bool Sockets::m_wsaStartupSucceeded = false;

// Initializes sockets (for example the winsock API).
void Sockets::initialize()
{
    // We only allow initialization on one thread at a time, to ensure that we 
    // only initialize WinSock once...
    std::lock_guard<std::mutex> lock(m_initializationMutex);

    // We update the initialization count...
    m_initializationCount++;
    if (m_initializationCount != 1)
    {
        // Sockets has already been initialized...
        return;
    }

    // This is the first initialization. We initialize Winsock, requesting version 2.2...
    WSADATA wsaData;
    auto versionRequested = MAKEWORD(2, 2);
    auto wsaError = WSAStartup(versionRequested, &wsaData);
    if (wsaError != 0) 
    {
        Logger::error(Utils::format("WSAStartup error: %d", wsaError));
    }

    // We confirm that we have version 2.2...
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
    {
        Logger::error("Could not find a usable version of Winsock.dll");
        WSACleanup();
    }
    else
    {
        m_wsaStartupSucceeded = true;
        Logger::info("WSAStartup: succeeded");
    }
}

// Cleans up sockets.
void Sockets:: cleanup()
{
    // We only allow cleanup on one thread at a time...
    std::lock_guard<std::mutex> lock(m_initializationMutex);

    // We check that cleanup() has not been called more times that initialize()...
    if (m_initializationCount == 0)
    {
        Logger::warn("Sockets::cleanup() has been called more times than Sockets::initialize()");
        return;
    }

    // We decrease the initialization reference count...
    m_initializationCount--;
    if (m_initializationCount != 0)
    {
        return;
    }

    // The reference count is zero so we should clean up...
    if (!m_wsaStartupSucceeded)
    {
        // WSAStartup was never run successfully, so there is nothing to clean up...
        return;
    }

    // We clean up...
    WSACleanup();
    Logger::info("WSACleanup: succeeded");
}
