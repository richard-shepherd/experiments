#pragma once
#include <cstdint>
#include "Callbacks.h"

namespace MessagingMesh
{
    // Forward declarations...
    class Connection;

    /// <summary>
    /// Manages a client subscription.
    /// Disposing the object unsubscribes.
    /// </summary>
    class Subscription
    {
    // Public methods:
    public:
        // Constructor.
        Subscription(Connection& connection, uint32_t subscriptionID, SubscriptionCallback callback);

        // Destructor.
        ~Subscription();

    // Private data...
    private:
        // Construction params...
        Connection& m_connection;
        uint32_t m_subscriptionID;
        SubscriptionCallback m_callback;
    };
} // namespace


