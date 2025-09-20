#include "Message.h"
#include "MessageImpl.h"
using namespace MessagingMesh;

Message::Message()
    : m_pImpl(new MessageImpl())
{
}

Message::~Message()
{
    delete m_pImpl;
}

void Message::addField(const std::string& name, const std::string& value)
{
    m_pImpl->addField(name, value);
}

void Message::addField(const std::string& name, int32_t value)
{
    m_pImpl->addField(name, value);
}

void Message::addField(const std::string& name, double value)
{
    m_pImpl->addField(name, value);
}

void Message::addField(const std::string& name, const ConstMessagePtr& value)
{
    m_pImpl->addField(name, value);
}

