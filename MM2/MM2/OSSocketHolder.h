#pragma once
#include <memory>
#include "uv.h"
#include "SharedPointers.h"
#include "UVUtils.h"

namespace MessagingMesh
{
    // Holds an OS socket object accessed via a mutex.
    // The mutex is used as these objects are passed between threads.
    class OSSocketHolder
    {
        // Public methods...
    public:
        // Creates an OSSocketHolder instance.
        static OSSocketHolderPtr create() { return OSSocketHolderPtr(new OSSocketHolder()); }

        // Sets the socket.
        void setSocket(uv_os_sock_t socket)
        {
            UVUtils::Lock lock(m_mutex);
            m_socket = socket;
        }

        // Gets the socket,
        uv_os_sock_t getSocket() const
        {
            UVUtils::Lock lock(m_mutex);
            return m_socket;
        }

    // Private functions...
    private:
        // Constructor.
        // NOTE: The constructor is private. Use OSSocketHolder::create() to create an instance.
        OSSocketHolder() :
            m_socket{}
        {
            // We create the mutex...
            m_mutex = std::make_unique<uv_mutex_t>();
            uv_mutex_init(m_mutex.get());
        }

    // Private data...
    private:
        // OS socket.
        uv_os_sock_t m_socket;

        // Mutex.
        std::unique_ptr<uv_mutex_t> m_mutex;
    };
} // namespace

