#pragma once
#include <memory>
#include <string>
#include "uv.h"
#include "NetworkData.h"

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

        // A UV write request plus the buffer it is writing.
        struct WriteRequest
        {
            // Constructor from NetworkData. 
            // The buffer points to the data in the NetworkData, and we hold a reference
            // to the NetworkData to ensure that its lifetime matches the write request.
            WriteRequest(NetworkDataPtr pNetworkData) :
                write_request{},
                m_pNetworkData(pNetworkData)
            {
                buffer.base = pNetworkData->getData();
                buffer.len = pNetworkData->getDataSize();
            }

            // Constructor specifying a buffer size.
            // We allocate the buffer and release it in the destructor.
            WriteRequest(size_t bufferSize) :
                write_request{},
                m_pNetworkData(nullptr)
            {
                buffer.base = new char[bufferSize];
                buffer.len = (ULONG)bufferSize;
            }

            // Destructor.
            ~WriteRequest()
            {
                // If we allocated the buffer (ie, if we are not using the
                // buffer from a NetworkData), we release it.
                if (!m_pNetworkData)
                {
                    delete[] buffer.base;
                }
            }

            uv_write_t write_request;
            uv_buf_t buffer;
            NetworkDataPtr m_pNetworkData;
        };

    // Public functions...
    public:
        // Gets peer IP info for a tcp handle.
        // (Peer info is the address and port of the remote end of the socket connection.)
        static IPInfo getPeerIPInfo(uv_tcp_t* pTCPHandle);

        // Allocates a buffer for a UV read from a socket.
        static void allocateBufferMemory(uv_handle_t* pHandle, size_t suggested_size, uv_buf_t* pBuffer);

        // Releases a buffer.
        static void releaseBufferMemory(const uv_buf_t* pBuffer);

        // Allocates a write request.
        static WriteRequest* allocateWriteRequest(NetworkDataPtr pNetworkData);

        // Allocates a write request.
        static WriteRequest* allocateWriteRequest(size_t bufferSize);

        // Releases a write request.
        static void releaseWriteRequest(WriteRequest* pWriteRequest);
    };

} // namespace

