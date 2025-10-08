#pragma once
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
            Lock(uv_mutex_t* pHandle) :
                m_pHandle(pHandle)
            {
                uv_mutex_lock(m_pHandle);
            }
            ~Lock()
            {
                uv_mutex_unlock(m_pHandle);
            }
        private:
            uv_mutex_t* m_pHandle;
        };
        
        
        
    };

} // namespace

