#pragma once
#include <string>
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
        // Creates a Socket instance.
        static SocketPtr create() { return SocketPtr(new Socket()); }

        // Destructor.
        ~Socket();

        // Connects a client socket to the IP address and port specified.
        void connectIP(const std::string& ipAddress, int port);

    private:
        // Constructor.
        // NOTE: The constructor is private. Use Socket::create() to create an instance.
        Socket();
    };

} // namespace
