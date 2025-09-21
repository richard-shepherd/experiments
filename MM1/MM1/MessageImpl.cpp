#include "MessageImpl.h"
#include "Field.h"
#include "Buffer.h"
using namespace MessagingMesh;

MessageImpl::MessageImpl()
{
}

MessageImpl::~MessageImpl()
{
}

void MessageImpl::addField(const std::string& name, const std::string& value)
{
    addField(name, [&value](const FieldPtr& field) {field->setString(value);});
}

void MessageImpl::addField(const std::string& name, int32_t value)
{
    addField(name, [&value](const FieldPtr& field) {field->setSignedInt32(value);});
}

void MessageImpl::addField(const std::string& name, double value)
{
    addField(name, [&value](const FieldPtr& field) {field->setDouble(value);});
}

void MessageImpl::addField(const std::string& name, const ConstMessagePtr& value)
{
    addField(name, [&value](const FieldPtr& field) {field->setMessage(value);});
}

/// <summary>
/// Private addField helper used by the public addField methods to create
/// and set up a field and add it to the message.
/// </summary>
void MessageImpl::addField(const std::string& name, std::function<void(const FieldPtr&)> valueSetter)
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

void MessageImpl::serialize(Buffer& buffer) const
{
    // We write the number of fields...
    auto fieldCount = static_cast<int32_t>(m_fields.size());
    buffer.write(fieldCount);

    // We write each field...
    for (auto& field : m_fields)
    {
        buffer.write(field);
    }
}
