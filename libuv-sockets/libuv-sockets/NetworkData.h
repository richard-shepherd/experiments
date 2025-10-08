#pragma once
#include <cstdint>

namespace MessagingMesh
{
    /// <summary>
    /// Data sent from the client to the server or from the server to the client.
    /// 
    /// This is a sort-of 'meta-protocol' for data sent across the network, just
    /// consisting of the size of the data and a number of bytes to be sent.
    /// 
    /// When the client or server is receiving a stream of bytes it can split it
    /// into individual messages based on the size.
    /// 
    /// The size is an int32_t so the maximum size for the data is ~2GB.
    /// 
    /// NOTE: This structure does not manage the memory for the data it holds.
    /// TODO: Think about ownership of memory. Should it be managed here?
    /// </summary>
    struct NetworkData
    {
        int32_t size;
        char* data;
    };

}  // namespace


