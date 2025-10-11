#pragma once
#include <cstdint>
#include "SharedPointers.h"

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
    /// NOTE: NetworkData owns the memory it holds. When it goes out of scope, the
    ///       memory will be released.
    /// </summary>
    class NetworkData
    {
    // Public methods...
    public:
        // Creates a NetworkData instance.
        static NetworkDataPtr create(int32_t size) { return NetworkDataPtr(new NetworkData(size)); }

        // Destructor.
        ~NetworkData();

    private:
        // Constructor.
        // NOTE: The constructor is private. Use NetworkData::create() to create an instance.
        NetworkData(int32_t size);

    // Private data...
    private:
        // The network data size.
        int32_t m_size;

        // Array of char of the size we are managing.
        char* m_pData;
    };

}  // namespace


