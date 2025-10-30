#pragma once
#include <map>
#include <string>
#include "SharedPointers.h"

namespace MessagingMesh
{
    /// <summary>
    /// Manages a messaging-mesh service. 
    /// 
    /// A service is a 'namespace' of messaging. Clients connecting to a service
    /// can send messages to each other, with message sent subjects only matching
    /// to subscriptions to the same service.
    /// 
    /// UV loop and thread
    /// ------------------
    /// Each service runs it own UV loop, so message processing for each service is
    /// managed on its own thread. As all updates on the UV loop take place on the
    /// (single) UV loop thread, this means that we do not have to lock service
    /// specific code such as the subject-matching engine.
    /// </summary>
    class ServiceManager
    {
    // Public methods...
    public:
        // Constructor.
        ServiceManager(const std::string& serviceName);

        // Destructor.
        ~ServiceManager() = default;

    // Private data...
    private:
        // The service name...
        std::string m_serviceName;

        // UV loop for processing client messages...
        UVLoopPtr m_pUVLoop;

        // Client sockets, keyed by socket name...
        std::map<std::string, SocketPtr> m_clientSockets;
    };
} // namespace

