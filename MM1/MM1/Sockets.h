#pragma once
#include <atomic>

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
        static std::atomic<int> m_initializationCount;
    };

} // namespace


