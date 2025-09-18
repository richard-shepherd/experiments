#include <functional>
#include "Message.h"
using namespace MessagingMesh;

Message::Message()
{
}

Message::~Message()
{
}

void Message::addField(const std::string& name, int32_t value)
{
    addField(name, [&value](const Field::Ptr& field) {field->setSignedInt32(value);});
}

void Message::addField(const std::string& name, double value)
{
    addField(name, [&value](const Field::Ptr& field) {field->setDouble(value);});
}

void Message::addField(const std::string& name, const Message::Ptr& value)
{
    addField(name, [&value](const Field::Ptr& field) {field->setMessage(value);});
}

void Message::addField(const std::string& name, std::function<void(const Field::Ptr&)> valueSetter)
{
    // We create a field with the specified name...
    auto field = Field::create(name);

    // We call the function (lambda) to set the value...
    valueSetter(field);

    // We add the field to the list of fields...
    m_fields.push_back(field);

    // We add the field to the map of name->first-field-for-name...
    m_mapNameToField.insert({ name, field });
}


