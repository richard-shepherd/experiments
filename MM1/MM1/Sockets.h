#pragma once
#include <atomic>
#include <mutex>

namespace MessagingMesh
{
    /// <summary>
    /// Sockets wrapper.
    /// </summary>
    class Sockets
    {
    // Static methods...
    public:
        // Initializes sockets (for example the winsock API).
        static void initialize();

        // Cleans up sockets.
        static void cleanup();

    private:
        // The number of initializations and a mutex to protect it. Initializations are
        // reference counted. We clean up when initialization-count goes to zero.
        static std::mutex m_initializationMutex;
        static int m_initializationCount;

        // True if WSAStartup succeeded...
        static bool m_wsaStartupSucceeded;
    };

} // namespace


