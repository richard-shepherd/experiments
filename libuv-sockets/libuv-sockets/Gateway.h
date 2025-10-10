#pragma once
#include <memory>
#include "UVLoopThread.h"
#include "Socket.h"
#include "SharedPointers.h"

namespace MessagingMesh
{
    // Forward declarations...
    class UVLoopThread;

    /// <summary>
    /// Manages a messaging-mesh gateway.
    /// </summary>
    class Gateway : public Socket::ICallback
    {
    // Public methods...
    public:
        // Constructor.
        Gateway(int port);

        // Destructor.
        ~Gateway();

    // Socket::ICallback implementation...
    public:
        // Called when data has been received on the socket.
        // Called on the UV loop thread.
        void onDataReceived(const NetworkData& networkData);

        // Called when a new client connection has been made to a listening socket.
        // Called on the UV loop thread.
        void onNewConnection(SocketPtr& clientSocket);

    // Private data...
    private:
        // The port on which we listen for client connections.
        int m_port;

        // Thread managing the main uv loop for the gateway.
        UVLoopThread m_loopThread;

        // Socket listening for incoming connections.
        SocketPtr m_listeningSocket;
    };

} // namespace

