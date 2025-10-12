#include <algorithm>
#include "NetworkData.h"
using namespace MessagingMesh;

// Constructor.
// NOTE: The constructor is private. Use NetworkData::create() to create an instance.
NetworkData::NetworkData() :
    m_size(-1),
    m_pDataBuffer(nullptr),
    m_dataBufferPosition(0),
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
int NetworkData::read(const char* pBuffer, int bufferSize, int bufferPosition)
{
    // See also the comments in Socket::onDataReceived() about how data for a message
    // can be received across multiple updates.

    // If we already have all the data we need, there is nothing to do...
    if (m_hasAllData)
    {
        return 0;
    }

    // We make sure that we have the message size...
    int bytesRead = readSize(pBuffer, bufferSize, bufferPosition);
    if (m_size == -1)
    {
        // We do not (yet) have the size...
        return bytesRead;
    }
    bufferPosition += bytesRead;

    // We find the number of bytes we need. This may not be the same as
    // the size of the message, as we may have already read from of the
    // data from previous updates.
    int sizeRequired = m_size - m_dataBufferPosition;

    // We find how much data there is available in the buffer and
    // work out how many bytes to read from the buffer...
    int sizeAvailable = bufferSize - bufferPosition;
    int sizeToRead = std::min(sizeRequired, sizeAvailable);

    // We copy the data into our buffer...
    memcpy(m_pDataBuffer + m_dataBufferPosition, pBuffer + bufferPosition, sizeToRead);
    bytesRead += sizeToRead;

    // We update the data buffer position. It may be that we still have not read the
    // whole message and we could need this position so we know where to append data
    // from future updates...
    m_dataBufferPosition += sizeToRead;

    // We check if we have the whole message...
    if (m_dataBufferPosition == m_size)
    {
        m_hasAllData = true;
    }

    return bytesRead;
}

// Reads the message size (or as much as can be read) from the buffer.
int NetworkData::readSize(const char* pBuffer, int bufferSize, int bufferPosition)
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
        m_sizeBuffer[m_sizeBufferPosition++] = pBuffer[bufferPosition++];
        bytesRead++;
    }

    // If we have read all the bytes for the size, we get the size...
    if (m_sizeBufferPosition == SIZE_SIZE)
    {
        // We copy the size buffer to the m_size field. (We can do this as
        // the messaging-mesh network protocol for int32 is little-endian.)
        memcpy(&m_size, &m_sizeBuffer[0], SIZE_SIZE);

        // We allocate the data buffer for the size...
        m_pDataBuffer = new char[m_size];
    }

    // We return the number of bytes read from the buffer (which may have not 
    // been enough to fully parse the size)...
    return bytesRead;
}
