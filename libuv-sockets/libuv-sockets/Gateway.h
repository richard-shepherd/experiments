#pragma once
#include <memory>
#include "UVLoopThread.h"

namespace MessagingMesh
{
    // Forward declarations...
    class UVLoopThread;

    /// <summary>
    /// Manages a messaging-mesh gateway.
    /// </summary>
    class Gateway
    {
    // Public methods...
    public:
        // Constructor.
        Gateway();

    // Private data...
    private:
        // Thread managing the main uv loop for the gateway...
        UVLoopThread m_loopThread;
    };

} // namespace

