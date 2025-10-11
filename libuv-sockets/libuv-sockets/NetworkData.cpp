#include "NetworkData.h"
using namespace MessagingMesh;

// Constructor.
// NOTE: The constructor is private. Use NetworkData::create() to create an instance.
NetworkData::NetworkData() :
    m_size(-1),
    m_pDataBuffer(nullptr),
    m_dataBufferPosition(-1),
    m_hasAllData(false),
    m_sizeBuffer{},
    m_sizeBufferPosition(0)
{
}

// Destructor.
NetworkData::~NetworkData()
{
    delete[] m_pDataBuffer;
}

// Returns true if we hold all data for the message, false if not.
bool NetworkData::hasAllData() const
{
    return m_hasAllData;
}

// Reads data from the buffer until we have all the data for this message
// or until we have consumed all the available data in the buffer.
// Returns the number of bytes read from the buffer.
int NetworkData::read(const uv_buf_t* pBuffer, int bufferSize, int bufferPosition)
{
    // We make sure that we have the message size...
    int bytesRead = readSize(pBuffer, bufferSize, bufferPosition);
    if (m_size == -1)
    {
        // We do not (yet) have the size...
        return bytesRead;
    }
    bufferPosition += bytesRead;

    // 





    return 0;
}

// Reads the message size (or as much as can be read) from the buffer.
int NetworkData::readSize(const uv_buf_t* pBuffer, int bufferSize, int bufferPosition)
{
    // We check if we already have the size...
    if (m_size != -1)
    {
        return 0;
    }

    // We read as many bytes as we can for the size from the buffer...
    int bytesRead = 0;
    while (m_sizeBufferPosition < SIZE_SIZE)
    {
        if (bufferPosition >= bufferSize) break;
        m_sizeBuffer[m_sizeBufferPosition++] = pBuffer->base[bufferPosition++];
        bytesRead++;
    }

    // If we have read all the bytes for the size, we get the size...
    if (m_sizeBufferPosition == SIZE_SIZE)
    {
        // We copy the size buffer to the m_size field. (We can do this as
        // the network protocol for the size is little-endian.)
        memcpy(&m_size, &m_sizeBuffer[0], SIZE_SIZE);

        // We allocate the data buffer for the size...
        m_pDataBuffer = new char[m_size];
    }

    // We return the number of bytes read from the buffer (which may have not 
    // been enough to fully parse the size)...
    return bytesRead;
}
