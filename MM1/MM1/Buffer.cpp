#include "Buffer.h"
using namespace MessagingMesh;

Buffer::Buffer()
{
    m_buffer.resize(INITIAL_SIZE);
}

Buffer::~Buffer()
{
}


