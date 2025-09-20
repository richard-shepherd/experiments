#include "Field.h"
#include "Exception.h"
#include "SharedPointers.h"
#include "Buffer.h"
using namespace MessagingMesh;

Field::Field()
: m_dataType(NOT_SET)
{
}

Field::~Field()
{
}

void Field::setName(const std::string& name)
{
    m_name = name;
}

const std::string& Field::getName() const
{
    return m_name;
}

void Field::setSignedInt32(int32_t value)
{
    m_dataType = SIGNED_INT32;
    m_data_int32 = value;
}

int32_t Field::getSignedInt32() const
{
    if (m_dataType != SIGNED_INT32)
    {
        throw Exception("Field '" + m_name + "' is not a signed int32");
    }
    return m_data_int32;
}

void Field::setDouble(double value)
{
    m_dataType = DOUBLE;
    m_data_double = value;
}

double Field::getDouble() const
{
    if (m_dataType != DOUBLE)
    {
        throw Exception("Field '" + m_name + "' is not a double");
    }
    return m_data_double;
}

void Field::setMessage(const MessagePtr& value)
{
    m_dataType = MESSAGE;
    m_data_message = value;
}

ConstMessagePtr Field::getMessage() const
{
    if (m_dataType != MESSAGE)
    {
        throw Exception("Field '" + m_name + "' is not a message");
    }
    return m_data_message;
}

void Field::serialize(Buffer& buffer) const
{
    // We serialize the field name...
    buffer.write(m_name);

    // We serialize the data type...
    buffer.write(static_cast<unsigned char>(m_dataType));

    // We serialize the data...
    switch (m_dataType)
    {
    case SIGNED_INT32:
        buffer.write(m_data_int32);
        break;

    case DOUBLE:
        buffer.write(m_data_double);
        break;

    default:
        throw Exception("Field::serialize data-type not handled");
    }
}
