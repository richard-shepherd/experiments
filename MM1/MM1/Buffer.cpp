#include "Buffer.h"
#include "Field.h"
#include "Exception.h"
using namespace MessagingMesh;

Buffer::Buffer()
{
    m_data.resize(INITIAL_SIZE);
}

Buffer::~Buffer()
{
}

void Buffer::write(unsigned char item)
{
    writeCopyable(item);
}

void Buffer::write(int32_t item)
{
    writeCopyable(item);
}

void Buffer::write(double item)
{
    writeCopyable(item);
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

void Buffer::write(const void* p, size_t size)
{
    // We make sure that the buffer can hold the new data...
    checkBufferSize(size);

    // We add the data to the buffer...
    memcpy(&m_data[m_position], p, size);

    // We update the position and data size... 
    updatePosition(size);
}

void Buffer::write(const ConstFieldPtr& item)
{
    // We do not write a field directly to the buffer. Instead we call its serialize()
    // method which will call back into this buffer to write the data depending on the
    // type managed by the field...
    item->serialize(*this);
}

template <typename T> 
void Buffer::writeCopyable(const T& item)
{
    // We make sure that the buffer can hold the new data...
    size_t size = sizeof(T);;
    checkBufferSize(size);

    // We add the data to the buffer...
    memcpy(&m_data[m_position], &item, size);

    // We update the position and data size... 
    updatePosition(size);
}

void Buffer::checkBufferSize(size_t bytesRequired)
{
    // We check if we can fit the bytes-required into the buffer at the current position...
    auto size = m_data.size();
    if (m_position + bytesRequired <= size)
    {
        // The bytes will fit...
        return;
    }

    // The bytes will not fit, so we expand the buffer by doubling its size...
    if (size >= INT32_MAX)
    {
        throw Exception("Buffer is at max capacity");
    }
    m_data.resize(size * 2);

    // We re-check that the bytes-required will fit with the new size...
    checkBufferSize(bytesRequired);
}

void Buffer::updatePosition(size_t bytesWritten)
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



