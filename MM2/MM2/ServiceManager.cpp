#include "ServiceManager.h"
#include "UVLoop.h"
using namespace MessagingMesh;

// Constructor.
ServiceManager::ServiceManager(const std::string& serviceName) :
    m_serviceName(serviceName),
    m_pUVLoop(UVLoop::create(serviceName))
{
}


