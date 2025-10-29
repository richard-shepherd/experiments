#include "ConnectionImpl.h"
#include "UVLoop.h"
#include "Socket.h"
#include "Utils.h"
#include "NetworkMessage.h"
#include "Message.h"
using namespace MessagingMesh;

// Constructor.
ConnectionImpl::ConnectionImpl(const std::string& hostname, int port, const std::string& service) :
    m_hostname(hostname),
    m_port(port),
    m_service(service)
{
    // We create the UV loop for client messaging...
    auto name = Utils::format("MM-%s", service.c_str());
    m_pUVLoop = UVLoop::create(name);

    // We create the socket to connect to the gateway...
    m_pSocket = Socket::create(m_pUVLoop);

    // We connect to the gateway...
    m_pUVLoop->marshallEvent(
        [this](uv_loop_t* /*pLoop*/)
        {
            m_pSocket->connect(m_hostname, m_port);
        }
    );

    // We send a CONNECT message...
    NetworkMessage connectMessage;
    connectMessage.getHeader().setAction(NetworkMessageHeader::Action::CONNECT);
    connectMessage.getMessage()->addField("SERVICE", m_service);
    sendNetworkMessage(connectMessage);
}

// Sends a network-message to the gateway.
void ConnectionImpl::sendNetworkMessage(const NetworkMessage& networkMessage)
{
    // We serialize the message and send it...
    auto pBuffer = Buffer::create();
    networkMessage.serialize(*pBuffer);
    m_pSocket->write(pBuffer);
}

