#pragma once
#include <string>
#include "uv.h"
#include "SharedPointers.h"
#include "ThreadsafeConsumableVector.h"

namespace MessagingMesh
{
    // Forward declarations...
    class NetworkData;
    class UVLoop;

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
        static SocketPtr create(UVLoop& uvLoop) { return SocketPtr(new Socket(uvLoop)); }

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

        // Writes data to the socket.
        void write(NetworkDataPtr pNetworkData);

        // Queues data to be written to the socket.
        // Can be called from any thread, not just from the uv loop thread.
        // Queued writes will be coalesced into one network update.
        void queueWrite(NetworkDataPtr pNetworkData);

        // Gets the socket handle.
        int getSocketHandle() const;

    // Private functions...
    private:
        // Constructor.
        // NOTE: The constructor is private. Use Socket::create() to create an instance.
        Socket(UVLoop& uvLoop);

        // Called at the client side when a client connect request has completed.
        void onConnectCompleted(uv_connect_t* pRequest, int status);

        // Called at the server side when a new client conection is received.
        void onNewConnection(uv_stream_t* server, int status);

        // Called when data has been received on a socket.
        void onDataReceived(uv_stream_t* pClientStream, ssize_t bufferSize, const uv_buf_t* pBuffer);

        // Called when a write request has completed.
        void onWriteCompleted(uv_write_t* pRequest, int status);

        // Sends a coalesced network message for all queued writes.
        void processQueuedWrites();

    // Private data...
    private:
        // The socket's name (made from its connection info).
        std::string m_name;

        // The uv loop managing the socket.
        UVLoop& m_uvLoop;

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

        // Data queued for writing.
        ThreadsafeConsumableVector<NetworkDataPtr> m_queuedWrites;
    };

} // namespace
