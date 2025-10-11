#include "NetworkData.h"
using namespace MessagingMesh;

// Constructor.
// NOTE: The constructor is private. Use NetworkData::create() to create an instance.
NetworkData::NetworkData() :
    m_size(-1),
    m_pData(nullptr),
    m_hasAllData(false),
    m_sizeBuffer{},
    m_sizeBufferPosition(0)
{
}

// Destructor.
NetworkData::~NetworkData()
{
    delete[] m_pData;
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
    return 0;
}
