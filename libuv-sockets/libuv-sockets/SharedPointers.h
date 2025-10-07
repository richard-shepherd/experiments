#pragma once
#include <memory>

namespace MessagingMesh
{
    // Forward declarations...
    class Socket;

    // Shared pointer to a Socket.
    typedef std::shared_ptr<Socket> SocketPtr;

} // namespace
