#include "FieldImpl.h"
#include "Buffer.h"
#include "Exception.h"
using namespace MessagingMesh;

// Checks that the data-type we hold is an the type specified.
#define CHECK_DATA_TYPE(x) if(m_dataType != x) throw Exception("Field '" + m_name + "' is not a " + #x)

FieldImpl::FieldImpl()
    : m_dataType(Field::NOT_SET)
{
}

FieldImpl::~FieldImpl()
{
}

void FieldImpl::setName(const std::string& name)
{
    m_name = name;
}

// Gets the field's name.
const std::string& FieldImpl::getName() const
{
    return m_name;
}

const std::string& FieldImpl::getString() const 
{
    CHECK_DATA_TYPE(Field::STRING);
    return m_data_string;
}

void FieldImpl::setString(const std::string& value) 
{
    m_dataType = Field::STRING;
    m_data_string = value;
}

int32_t FieldImpl::getSignedInt32() const
{
    CHECK_DATA_TYPE(Field::SIGNED_INT32);
    return m_data_int32;
}

void FieldImpl::setSignedInt32(int32_t value)
{
    m_dataType = Field::SIGNED_INT32;
    m_data_int32 = value;
}

double FieldImpl::getDouble() const
{
    CHECK_DATA_TYPE(Field::DOUBLE);
    return m_data_double;
}

void FieldImpl::setDouble(double value)
{
    m_dataType = Field::DOUBLE;
    m_data_double = value;
}

const ConstMessagePtr& FieldImpl::getMessage() const
{
    CHECK_DATA_TYPE(Field::MESSAGE);
    return m_data_message;
}

void FieldImpl::setMessage(const ConstMessagePtr& value)
{
    m_dataType = Field::MESSAGE;
    m_data_message = value;
}

void FieldImpl::serialize(Buffer& buffer) const
{
    // We serialize the field name...
    buffer.write(m_name);

    // We serialize the data type...
    buffer.write(static_cast<unsigned char>(m_dataType));

    // We serialize the data...
    switch (m_dataType)
    {
    case Field::STRING:
        buffer.write(m_data_string);
        break;

    case Field::SIGNED_INT32:
        buffer.write(m_data_int32);
        break;

    case Field::DOUBLE:
        buffer.write(m_data_double);
        break;

    default:
        throw Exception("Field::serialize data-type not handled");
    }
}
