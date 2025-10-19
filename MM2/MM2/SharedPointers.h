#pragma once
#include <memory>

namespace MessagingMesh
{
    // Forward declarations...
    class NetworkData;
    class Socket;
    class OSSocketHolder;
    class UVLoop;

    // Shared pointer to a NetworkData.
    typedef std::shared_ptr<NetworkData> NetworkDataPtr;

    // Shared pointer to a Socket.
    typedef std::shared_ptr<Socket> SocketPtr;

    // Shared pointer to an OSSocketHolder.
    typedef std::shared_ptr<OSSocketHolder> OSSocketHolderPtr;

    // Shared pointer to a UVLoop.
    typedef std::shared_ptr<UVLoop> UVLoopPtr;

} // namespace
