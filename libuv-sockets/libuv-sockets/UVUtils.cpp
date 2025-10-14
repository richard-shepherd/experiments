#include "UVUtils.h"
#include "Utils.h"
#include "Logger.h"
#include "NetworkData.h"
using namespace MessagingMesh;

// WriteRequest constructor.
UVUtils::WriteRequest::WriteRequest(const NetworkDataPtr& pNetworkData) :
    write_request{},
    buffer{},
    m_pNetworkData(pNetworkData)
{
    buffer.base = pNetworkData->getData();
    buffer.len = pNetworkData->getDataSize();
}

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

// Allocates buffer memory for a UV read from a socket.
void UVUtils::allocateBufferMemory(uv_handle_t* handle, size_t suggested_size, uv_buf_t* pBuffer)
{
    pBuffer->base = new char[suggested_size];
    pBuffer->len = (ULONG)suggested_size;
}

// Releases memory for a buffer.
void UVUtils::releaseBufferMemory(const uv_buf_t* pBuffer)
{
    delete[] pBuffer->base;
}

// Allocates a write request.
UVUtils::WriteRequest* UVUtils::allocateWriteRequest(const NetworkDataPtr& pNetworkData)
{
    return new WriteRequest(pNetworkData);
}

// Releases a write request.
void UVUtils::releaseWriteRequest(WriteRequest* pWriteRequest)
{
    delete pWriteRequest;
}

