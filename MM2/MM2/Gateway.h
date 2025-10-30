#pragma once
#include <memory>
#include <map>
#include "UVLoop.h"
#include "Socket.h"
#include "SharedPointers.h"
#include "ServiceManager.h"

namespace MessagingMesh
{
    /// <summary>
    /// Manages a messaging-mesh gateway.
    /// 
    /// UV loop and threading
    /// ---------------------
    /// The gateway uses a UV loop to manage a socket listeing for client connections.
    /// All methods except the constructor and destructor will be called from the UV
    /// loop's thread.
    /// 
    /// Services
    /// --------
    /// When clients connect using the Connection class, they specify a service. Each service
    /// is a messaging group where clients can send and receive messages with each other. All
    /// messages sent or received by the Connection are managed within the service. 
    /// 
    /// Message subjects are scoped by the service. So a client subscribing to subject A.B on
    /// service SERVICE-1 will only recieve updates to A.B sent by other clients connected to
    /// SERVICE-1. This allows a single gateway to manage messaging for multiple systems with
    /// no cross-talk of messaging between them, even if they use the same subject names.
    /// 
    /// One UV loop per service
    /// -----------------------
    /// The gateway has a loop which it uses to listen for new client connections. All initial
    /// connections are handled by this loop, regardless of service. At the point of the initial
    /// connection we do not yet know the service requested by the client and the client socket
    /// will be held in the pending-connection collection.
    /// 
    /// Shortly after making a connection the client will send a CONNECT message including the
    /// service name. The gateway will then find the ServiceManager for this service, or create
    /// one if this is the first client for the service. The client socket will be handed to the
    /// ServiceManager and removed from the pending-connection collectio.
    /// 
    /// Each ServiceManager runs its own UV loop, so all subsequent interactions with the client
    /// will be managed by that loop. This means that each service runs on its own thread.
    /// </summary>
    class Gateway : public Socket::ICallback
    {
    // Public methods...
    public:
        // Constructor.
        Gateway(int port);

        // Destructor.
        ~Gateway() = default;

    // Socket::ICallback implementation...
    public:
        // Called when a new client connection has been made to a listening socket.
        // Called on the GATEWAY thread.
        void onNewConnection(SocketPtr pClientSocket);

        // Called when data has been received on the socket.
        // Called on the thread of the client socket.
        void onDataReceived(const std::string& socketName, BufferPtr pBuffer);

        // Called when a socket has been disconnected.
        // Called on the socket's thread.
        void onDisconnected(const std::string& socketName);

    // Private functions...
    private:
        // Creates the socket to listen for client connections.
        void createListeningSocket();

    // Private data...
    private:
        // The port on which we listen for client connections.
        int m_port;

        // UV loop for listening for new client connections.
        UVLoopPtr m_pUVLoop;

        // Socket listening for incoming connections.
        SocketPtr m_listeningSocket;

        // Sockets for which we have not yet received a CONNECT message, keyed 
        // by socket name. We need to hold onto these to avoid the Sockets going
        // out of scope and being destructed.
        std::map<std::string, SocketPtr> m_pendingConnections;

        // Service managers, keyed by service name...
        std::map<std::string, ServiceManager> m_serviceManagers;
    };
} // namespace

