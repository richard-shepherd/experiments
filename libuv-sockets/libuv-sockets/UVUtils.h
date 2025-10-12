#pragma once
#include <memory>
#include <string>
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
            Lock(const std::unique_ptr<uv_mutex_t>& handle) : m_handle(handle) { uv_mutex_lock(m_handle.get()); }
            ~Lock() { uv_mutex_unlock(m_handle.get()); }
        private:
            const std::unique_ptr<uv_mutex_t>& m_handle;
        };

        // Holds parsed IP info.
        struct IPInfo
        {
            std::string Hostname;  // Hostname or IP address
            std::string Service;   // Service or port
        };
        
    // Public functions...
    public:
        // Gets peer IP info for a tcp handle.
        // (Peer info is the address and port of the remote end of the socket connection.)
        static IPInfo getPeerIPInfo(uv_tcp_t* pTCPHandle);

        // Creates a buffer. for example for writing to a socket.
        static uv_buf_t* createBuffer(size_t size);

        // Releases a buffer including both the buffer memory and the uv_buf_t itself.
        static void deleteBuffer(const uv_buf_t* pBuffer);

        // Allocates a buffer for a UV read from a socket.
        static void allocateBufferMemory(uv_handle_t* pHandle, size_t suggested_size, uv_buf_t* pBuffer);

        // Releases a buffer.
        static void releaseBufferMemory(const uv_buf_t* pBuffer);

        // Allocates a write request.
        static uv_write_t* allocateWriteRequest();

        // Releases a write request.
        static void releaseWriteRequest(uv_write_t* pWriteRequest);
    };

} // namespace

