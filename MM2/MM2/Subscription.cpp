#include "Subscription.h"
using namespace MessagingMesh;

// Constructor.
Subscription::Subscription(ConnectionImpl* pConnection, uint32_t subscriptionID, SubscriptionCallback callback) :
    m_pConnection(pConnection),
    m_subscriptionID(subscriptionID),
    m_callback(callback)
{
}

// Destructor.
Subscription::~Subscription()
{
}

