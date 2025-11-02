#include "Subscription.h"
using namespace MessagingMesh;

// Constructor.
Subscription::Subscription(Connection& connection, uint32_t subscriptionID, SubscriptionCallback callback) :
    m_connection(connection),
    m_subscriptionID(subscriptionID),
    m_callback(callback)
{
}

// Destructor.
Subscription::~Subscription()
{
}

