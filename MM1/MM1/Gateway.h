#pragma once

namespace MessagingMesh
{
    /// <summary>
    /// The broker for messaging mesh. Handles:
    /// - Connection requests from clients
    /// - Subscription requests
    /// - Publications of messages, routing them to matching client subscriptions
    /// </summary>
    class Gateway
    {
        // Public methods...
    public:
        // Constructor...
        Gateway(int port);
    };

} // namespace

