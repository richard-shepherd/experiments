#pragma once
#include <memory>
#include "uv.h"

namespace MessagingMesh
{
    /// <summary>
    /// Utils for use with libuv.
    /// </summary>
    class UVUtils
    {
    // Public types...
    public:

        // Locks a mutex in the constructor and releases the lock in the destructor.
        class Lock
        {
        public:
            Lock(const std::unique_ptr<uv_mutex_t>& handle) :
                m_handle(handle)
            {
                uv_mutex_lock(m_handle.get());
            }
            ~Lock()
            {
                uv_mutex_unlock(m_handle.get());
            }
        private:
            const std::unique_ptr<uv_mutex_t>& m_handle;
        };
        
        
        
    };

} // namespace

