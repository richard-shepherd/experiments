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
            // Called on the UV loop thread.
            virtual void onDataReceived(NetworkDataPtr networkData) = 0;

            // Called when a new client connection has been made to a listening socket.
            // Called on the UV loop thread.
            virtual void onNewConnection(SocketPtr clientSocket) = 0;
        };

    // Public methods...
    public:
        // Creates a Socket instance to be managed by the uv loop specified.
        static SocketPtr create(uv_loop_t* pLoop) { return SocketPtr(new Socket(pLoop)); }

        // Destructor.
        ~Socket();

        // Sets the callback.
        void setCallback(ICallback* pCallback);

        // Connects a server socket to listen on the specified port.
        void listen(int port);

        // Connects the socket by accepting a listen request received by the server.
        void accept(uv_stream_t* server);

        // Connects a client socket to the IP address and specified port.
        void connectIP(const std::string& ipAddress, int port);

    // Private functions...
    private:
        // Constructor.
        // NOTE: The constructor is private. Use Socket::create() to create an instance.
        Socket(uv_loop_t* pLoop);

        // Called at the client side when a client has connected to a server.
        void onConnect(uv_connect_t* pRequest, int status);

        // Called at the server side when a new client conection is received.
        void onNewConnection(uv_stream_t* server, int status);

        // Called when data has been received on a socket.
        void onDataReceived(uv_stream_t* pClientStream, ssize_t bufferSize, const uv_buf_t* pBuffer);

    // Private data...
    private:
        // The socket's name (made from its connection info).
        std::string m_name;

        // The uv loop managing the socket.
        uv_loop_t* m_pLoop;

        // The object on which we call callbacks.
        ICallback* m_pCallback;

        // UV socket handle.
        std::unique_ptr<uv_tcp_t> m_uvSocket;

    // Constants...
    private:
        // The maximum backlog of unprocessed incoming connections.
        const int MAX_INCOMING_CONNECTION_BACKLOG = 128;

        // The message being currently read (possibly across multiple onDataReceived callbacks).
        NetworkDataPtr m_currentMessage;
    };

} // namespace
