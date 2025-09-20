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

void Buffer::add(unsigned char item)
{
    // We make sure that the buffer can hold the new data...
    size_t size = 1;
    checkBufferSize(size);

    // We add the data to the buffer...
    m_data[m_position] = item;

    // We update the position and data size... 
    updatePosition(size);
}

void Buffer::add(const ConstFieldPtr& item)
{
    item->serialize(*this);
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



