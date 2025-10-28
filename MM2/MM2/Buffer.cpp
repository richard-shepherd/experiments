#include "Buffer.h"
#include "Field.h"
#include "Message.h"
#include "Exception.h"
#include "Logger.h"
#include "Utils.h"
using namespace MessagingMesh;

Buffer::Buffer()
{
}

Buffer::~Buffer()
{
    delete[] m_pBuffer;
}

// Gets the buffer.
char* Buffer::getBuffer() const
{
    // We update the data size, stored in the first four bytes.
    // This is the size of the data itself not including the bytes 
    // that hold the size.
    int32_t size = m_dataSize - SIZE_SIZE;
    std::memcpy(m_pBuffer, &size, sizeof(size));

    // We return the buffer...
    return m_pBuffer;
}

// INT8
int8_t Buffer::readInt8()
{
    int8_t result;
    readCopyable(result);
    return result;
}

void Buffer::write(int8_t item)
{
    writeCopyable(item);
}

// INT32
int32_t Buffer::readInt32()
{
    int32_t result;
    readCopyable(result);
    return result;
}

void Buffer::write(int32_t item)
{
    writeCopyable(item);
}

// UINT32
uint32_t Buffer::readUInt32()
{
    uint32_t result;
    readCopyable(result);
    return result;
}
void Buffer::write(uint32_t item)
{
    writeCopyable(item);
}

// DOUBLE
double Buffer::readDouble()
{
    double result;
    readCopyable(result);
    return result;
}

void Buffer::write(double item)
{
    writeCopyable(item);
}

// STRING
std::string Buffer::readString()
{
    // String are serialized as [length][chars].

    // We read the length...
    auto length = readInt32();

    // We check the buffer size...
    checkBufferSize_Read(length);

    // We create a string and read the data into it...
    std::string result(m_pBuffer + m_position, m_pBuffer + m_position + length);

    // We update the position...
    updatePosition_Read(length);

    return result;
}

void Buffer::write(const std::string& item)
{
    // String are serialized as [length][chars].

    // We write the length...
    auto length = static_cast<int32_t>(item.length());
    write(length);

    // We write the characters...
    write(item.c_str(), length);
}

// BYTE ARRAY
void Buffer::read(void* p, int32_t size)
{
    // We make sure that the buffer is large enough...
    checkBufferSize_Read(size);

    // We read the data from the buffer...
    std::memcpy(p, m_pBuffer + m_position, size);

    // We update the position... 
    updatePosition_Read(size);
}

void Buffer::write(const void* p, int32_t size)
{
    // We make sure that the buffer can hold the new data...
    checkBufferSize_Write(size);

    // We write the data to the buffer...
    std::memcpy(m_pBuffer + m_position, p, size);

    // We update the position and data size... 
    updatePosition_Write(size);
}

// FIELD
ConstFieldPtr Buffer::readField()
{
    // We create a new field and deserialize into it...
    auto field = Field::create();
    field->deserialize(*this);
    return field;
}

void Buffer::write(const ConstFieldPtr& item)
{
    // We call the field's serialize() method. This calls back into the buffer
    // to write the data for the field and the specific type it is managing...
    item->serialize(*this);
}

// MESSAGE
ConstMessagePtr Buffer::readMessage()
{
    // We create a new message and deserialize into it...
    auto message = Message::create();
    message->deserialize(*this);
    return message;
}

void Buffer::write(const ConstMessagePtr& item)
{
    // We call the message's serialize() method. This calls back into the buffer
    // to write the data for the message and the fields it is managing...
    item->serialize(*this);
}

template <typename T>
void Buffer::readCopyable(T& item)
{
    // We make sure that the buffer is large enough...
    size_t size = sizeof(T);
    checkBufferSize_Read(size);

    // We read the data from the buffer...
    std::memcpy(&item, m_pBuffer + m_position, size);

    // We update the position...
    updatePosition_Read(static_cast<int32_t>(size));
}

template <typename T>
void Buffer::writeCopyable(const T& item)
{
    // We make sure that the buffer can hold the new data...
    size_t size = sizeof(T);
    checkBufferSize_Write(size);

    // We write the data to the buffer...
    std::memcpy(m_pBuffer + m_position, &item, size);

    // We update the position and data size... 
    updatePosition_Write(static_cast<int32_t>(size));
}

void Buffer::checkBufferSize_Read(size_t bytesRequired)
{
    if (m_position + bytesRequired > m_dataSize)
    {
        // The buffer is not large enough to read the bytes requested...
        throw Exception("Buffer is not large enough to read requested data");
    }
}

void Buffer::checkBufferSize_Write(size_t bytesRequired)
{
    // We check if we can fit the bytes-required into the buffer at the current position...
    if (m_position + bytesRequired <= m_bufferSize)
    {
        // The bytes will fit...
        return;
    }

    // The bytes will not fit, so we expand the buffer...
    expandBuffer();

    // We re-check that the bytes-required will fit with the new size...
    checkBufferSize_Write(bytesRequired);
}

// Expands the buffer by doubling its size.
void Buffer::expandBuffer()
{
    if (m_bufferSize >= INT32_MAX)
    {
        throw Exception("Buffer is at max capacity");
    }

    // If the buffer has not yet been allocated, we allocate the initial size...
    if (!m_pBuffer)
    {
        m_pBuffer = new char[INITIAL_SIZE];
        m_bufferSize = INITIAL_SIZE;
        return;
    }

    // We create a new buffer double the size and copy the existing data into it...
    auto newBufferSize = m_bufferSize * 2;
    auto newBuffer = new char[newBufferSize];
    std::memcpy(newBuffer, m_pBuffer, m_bufferSize);

    // We delete the old buffer...
    delete[] m_pBuffer;

    // We use the new buffer...
    m_pBuffer = newBuffer;
    m_bufferSize = newBufferSize;
}

void Buffer::updatePosition_Read(int32_t bytesWritten)
{
    // We update the position...
    m_position += bytesWritten;
}

void Buffer::updatePosition_Write(int32_t bytesWritten)
{
    // We update the position...
    m_position += bytesWritten;

    // We update the data-size if the new position is more than the previous size.
    // (This may not be the case if the position has been manually changed to write
    // to an earlier point in the buffer.)
    if (m_position > m_dataSize)
    {
        m_dataSize = m_position;
    }
}

// Reads data from a network data buffer until we have all the data for
// a network message.
// Returns the number of bytes read from the buffer.
size_t Buffer::readNetworkMessage(const char* pBuffer, size_t bufferSize, size_t bufferPosition)
{
    // See also the comments in Socket::onDataReceived() about how data for a message
    // can be received across multiple updates.

    // If we already have all the data we need, there is nothing to do...
    if (m_hasAllData)
    {
        return 0;
    }

    // We make sure that we have the message size.
    // (This is a no-op if we already know the size.)
    size_t bytesRead = readNetworkMessageSize(pBuffer, bufferSize, bufferPosition);
    if (m_networkMessageSize == -1)
    {
        // We do not (yet) have the size...
        return bytesRead;
    }
    bufferPosition += bytesRead;

    // We find the number of bytes we need. This may not be the same as
    // the size of the message, as we may have already read from of the
    // data from previous updates.
    size_t sizeRequired = m_networkMessageSize - m_position + SIZE_SIZE;

    // We find how much data there is available in the buffer and
    // work out how many bytes to read from the buffer...
    size_t sizeAvailable = bufferSize - bufferPosition;
    size_t sizeToRead = std::min(sizeRequired, sizeAvailable);

    // We copy the data into our buffer...
    std::memcpy(m_pBuffer + m_position, pBuffer + bufferPosition, sizeToRead);
    bytesRead += sizeToRead;

    // We update the data buffer position. It may be that we still have not read the
    // whole message and we could need this position so we know where to append data
    // from future updates...
    m_position += static_cast<int32_t>(sizeToRead);

    // We check if we have the whole message...
    if (m_position == m_networkMessageSize + SIZE_SIZE)
    {
        m_hasAllData = true;
    }

    return bytesRead;
}

// Reads the network message size (or as much as can be read) from the buffer.
size_t Buffer::readNetworkMessageSize(const char* pBuffer, size_t bufferSize, size_t bufferPosition)
{
    // We check if we already have the size...
    if (m_networkMessageSize != -1)
    {
        return 0;
    }

    // We read as many bytes as we can for the size from the buffer...
    int32_t bytesRead = 0;
    while (m_networkMessageSizeBufferPosition < SIZE_SIZE)
    {
        if (bufferPosition >= bufferSize) break;
        m_networkMessageSizeBuffer[m_networkMessageSizeBufferPosition++] = pBuffer[bufferPosition++];
        bytesRead++;
    }

    // If we have read all the bytes for the size, we get the size...
    if (m_networkMessageSizeBufferPosition == SIZE_SIZE)
    {
        // We copy the size buffer to the network message size field. (We can do this as
        // the messaging-mesh network protocol for int32 is little-endian.)
        std::memcpy(&m_networkMessageSize, &m_networkMessageSizeBuffer[0], SIZE_SIZE);

        // We allocate the data buffer for the size, plus four bytes to 
        // hold the size itself...
        delete[] m_pBuffer;
        m_bufferSize = m_networkMessageSize + SIZE_SIZE;
        m_dataSize = m_bufferSize;
        m_pBuffer = new char[m_bufferSize];

        // We make sure that the position is four bytes from the start of the buffer.
        // The first four bytes are reserved for the size itself. The data will be
        // written after this.
        m_position = SIZE_SIZE;
    }

    // We return the number of bytes read from the buffer (which may have not 
    // been enough to fully parse the size)...
    return bytesRead;
}

