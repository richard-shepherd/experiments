#include <functional>
#include "Message.h"
#include "Field.h"
using namespace MessagingMesh;

Message::Message()
{
}

Message::~Message()
{
}

void Message::addField(const std::string& name, int32_t value)
{
    addField(name, [&value](const FieldPtr& field) {field->setSignedInt32(value);});
}

void Message::addField(const std::string& name, double value)
{
    addField(name, [&value](const FieldPtr& field) {field->setDouble(value);});
}

void Message::addField(const std::string& name, const MessagePtr& value)
{
    addField(name, [&value](const FieldPtr& field) {field->setMessage(value);});
}

/// <summary>
/// Private addField helper used by the public addField methods to create
/// and set up a field and add it to the message.
/// </summary>
void Message::addField(const std::string& name, std::function<void(const FieldPtr&)> valueSetter)
{
    // We create a field and set its name...
    auto field = Field::create();
    field->setName(name);

    // We call the function (lambda) to set the value...
    valueSetter(field);

    // We add the field to the list of fields...
    m_fields.push_back(field);

    // We add the field to the map of name->first-field-for-name...
    m_mapNameToField.insert({ name, field });
}


