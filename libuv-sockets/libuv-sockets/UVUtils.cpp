#include "UVUtils.h"
#include "Utils.h"
#include "Logger.h"
using namespace MessagingMesh;

// Gets peer IP info for a tcp handle.
// (Peer info is the address and port of the remote end of the socket connection.)
UVUtils::IPInfo UVUtils::getPeerIPInfo(uv_tcp_t* pTCPHandle)
{
    // We get the socket info...
    sockaddr_storage peerInfo;
    int peerInfoSize = sizeof(sockaddr_storage);
    uv_tcp_getpeername(pTCPHandle, (sockaddr*)&peerInfo, &peerInfoSize);

    // We find the hostname/ip-address and service/port...
    char hostname[NI_MAXHOST];
    char service[NI_MAXSERV];
    auto status = getnameinfo(
        (struct sockaddr*)&peerInfo,
        sizeof(struct sockaddr_storage),
        hostname,
        NI_MAXHOST,
        service,
        NI_MAXSERV,
        NI_NUMERICSERV);

    // We return the data in an IPInfo structure...
    IPInfo ipInfo;
    if (status == 0)
    {
        ipInfo.Hostname = hostname;
        ipInfo.Service = service;
    }
    else
    {
        Logger::error(Utils::format("getPeerInfo: getnameinfo error: %d", status));
    }
    return ipInfo;
}

// Creates a buffer. for example for writing to a socket.
uv_buf_t* UVUtils::createBuffer(size_t size)
{
    auto pBuffer = new uv_buf_t;
    pBuffer->base = new char[size];
    pBuffer->len = size;
    return pBuffer;
}

// Releases a buffer including both the buffer memory and the uv_buf_t itself.
void UVUtils::deleteBuffer(const uv_buf_t* pBuffer)
{
    delete[] pBuffer->base;
    delete pBuffer;
}

// Allocates buffer memory for a UV read from a socket.
void UVUtils::allocateBufferMemory(uv_handle_t* handle, size_t suggested_size, uv_buf_t* pBuffer)
{
    pBuffer->base = new char[suggested_size];
    pBuffer->len = suggested_size;
}

// Releases memory for a buffer.
void UVUtils::releaseBufferMemory(const uv_buf_t* pBuffer)
{
    delete[] pBuffer->base;
}

// Allocates a write request.
uv_write_t* UVUtils::allocateWriteRequest()
{
    return new uv_write_t;
}

// Releases a write request.
void UVUtils::releaseWriteRequest(uv_write_t* pWriteRequest)
{
    delete pWriteRequest;
}

