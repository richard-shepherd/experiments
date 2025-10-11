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
    /// consisting of the size of the data-buffer and the data-buffer itself.
    /// 
    /// The size is an int32_t so the maximum size for the data is ~2GB.
    /// 
    /// NOTE 1: The size does not include the size of the size itself.
    ///         For example a message containing an int32 will look like:
    ///         - size: 4
    ///         - data-buffer (int32): 4 bytes
    /// 
    /// NOTE 2: NetworkData owns the memory it holds. When it goes out of scope, the
    ///         memory will be released.
    /// RSSTODO: Do we want to own the memory when *sending* data?
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

        // Reads the message size (or as much as can be read) from the buffer.
        int readSize(const uv_buf_t* pBuffer, int bufferSize, int bufferPosition);

    // Private data...
    private:
        // The network data size.
        int32_t m_size;

        // The data buffer and the next position to read data into it.
        // (We may receive the data across multiple network updates.)
        char* m_pDataBuffer;
        int m_dataBufferPosition;

        // True if we have all data for the message, false if not.
        bool m_hasAllData;

        // Buffer when reading the size.
        // (We may receive the size across multiple network updates.)
        static const int SIZE_SIZE = 4;
        char m_sizeBuffer[SIZE_SIZE];
        int m_sizeBufferPosition;
    };

}  // namespace


