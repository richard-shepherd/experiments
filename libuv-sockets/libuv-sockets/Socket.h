#pragma once
#include <string>
#include "uv.h"
#include "SharedPointers.h"

namespace MessagingMesh
{
    // Forward declarations...
    class NetworkData;

    /// <summary>
    /// Manages a socket.
    /// 
    /// Can either be a client socket making a connection to a server
    /// or a server socket listening for client connections.
    /// </summary>
    class Socket
    {
    public:
        // Interface for socket callbacks.
        class ICallback
        {
        public:
            // Called when data has been received on the socket.
            virtual void onDataReceived(const NetworkData& networkData) = 0;

            // Called when a new client connection has been made to a listening socket.
            virtual void onNewConnection(const SocketPtr& clientSocket) = 0;
        };

    // Public methods...
    public:
        // Creates a Socket instance to be managed by the uv loop specified.
        static SocketPtr create(uv_loop_t* pLoop) { return SocketPtr(new Socket(pLoop)); }

        // Destructor.
        ~Socket();

        // Connects a client socket to the IP address and port specified.
        void connectIP(const std::string& ipAddress, int port);

    private:
        // Constructor.
        // NOTE: The constructor is private. Use Socket::create() to create an instance.
        Socket(uv_loop_t* pLoop);

    // Private data...
    private:
        // The uv loop managing the socket.
        uv_loop_t* m_pLoop;
    };

} // namespace
