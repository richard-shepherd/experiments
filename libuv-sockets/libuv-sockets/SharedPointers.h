#pragma once
#include <memory>

namespace MessagingMesh
{
    // Forward declarations...
    class NetworkData;
    class Socket;

    // Shared pointer to a NetworkData.
    typedef std::shared_ptr<NetworkData> NetworkDataPtr;

    // Shared pointer to a Socket.
    typedef std::shared_ptr<Socket> SocketPtr;

} // namespace
