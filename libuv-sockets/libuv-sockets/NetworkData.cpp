#include "NetworkData.h"
using namespace MessagingMesh;

// Constructor.
// NOTE: The constructor is private. Use NetworkData::create() to create an instance.
NetworkData::NetworkData(int32_t size) :
    m_pData(new char[size])
{
}

// Destructor.
NetworkData::~NetworkData()
{
    delete[] m_pData;
}

