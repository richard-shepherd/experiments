#include "Buffer.h"
#include "Field.h"
#include "Message.h"
#include "Exception.h"
using namespace MessagingMesh;

Buffer::Buffer()
{
    m_data.resize(INITIAL_SIZE);
}

Buffer::~Buffer()
{
}

// UNSIGNED CHAR
unsigned char Buffer::readUnsignedChar()
{
    unsigned char result;
    readCopyable(result);
    return result;
}

void Buffer::write(unsigned char item)
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
    std::string result(&m_data[m_position], &m_data[m_position] + length);

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
void Buffer::read(void* p, size_t size)
{
    // We make sure that the buffer is large enough...
    checkBufferSize_Read(size);

    // We read the data from the buffer...
    memcpy(p, &m_data[m_position], size);

    // We update the position... 
    updatePosition_Read(size);
}

void Buffer::write(const void* p, size_t size)
{
    // We make sure that the buffer can hold the new data...
    checkBufferSize_Write(size);

    // We write the data to the buffer...
    memcpy(&m_data[m_position], p, size);

    // We update the position and data size... 
    updatePosition_Write(size);
}

// FIELD
void Buffer::write(const ConstFieldPtr& item)
{
    // We call the field's serialize() method. This calls back into the buffer
    // to write the data for the field and the specific type it is managing...
    item->serialize(*this);
}

// MESSAGE
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
    memcpy(&item, &m_data[m_position], size);

    // We update the position...
    updatePosition_Read(size);
}

template <typename T>
void Buffer::writeCopyable(const T& item)
{
    // We make sure that the buffer can hold the new data...
    size_t size = sizeof(T);
    checkBufferSize_Write(size);

    // We write the data to the buffer...
    memcpy(&m_data[m_position], &item, size);

    // We update the position and data size... 
    updatePosition_Write(size);
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
    auto dataSize = m_data.size();
    if (m_position + bytesRequired <= dataSize)
    {
        // The bytes will fit...
        return;
    }

    // The bytes will not fit, so we expand the buffer by doubling its size...
    if (dataSize >= INT32_MAX)
    {
        throw Exception("Buffer is at max capacity");
    }
    m_data.resize(dataSize * 2);

    // We re-check that the bytes-required will fit with the new size...
    checkBufferSize_Write(bytesRequired);
}

void Buffer::updatePosition_Read(size_t bytesWritten)
{
    // We update the position...
    m_position += bytesWritten;
}

void Buffer::updatePosition_Write(size_t bytesWritten)
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



