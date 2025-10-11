#pragma once
#include <cstdint>
#include "uv.h"
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
        static NetworkDataPtr create() { return NetworkDataPtr(new NetworkData()); }

        // Destructor.
        ~NetworkData();

        // Returns true if we hold all data for the message, false if not.
        bool hasAllData() const;

        // Reads data from the buffer until we have all the data for this message
        // or until we have consumed all the available data in the buffer.
        // Returns the number of bytes read from the buffer.
        int read(const uv_buf_t* pBuffer, int bufferSize, int bufferPosition);

    private:
        // Constructor.
        // NOTE: The constructor is private. Use NetworkData::create() to create an instance.
        NetworkData();

    // Private data...
    private:
        // The network data size.
        int32_t m_size;

        // Array of char of the size we are managing.
        char* m_pData;

        // True if we have all data for the message, false if not.
        bool m_hasAllData;

        // Buffer when reading the size (which can comes across multiple network updates).
        char m_sizeBuffer[4];
        int m_sizeBufferPosition;
    };

}  // namespace


