#include "Field.h"
#include "MessagingMeshExceptions.h"
using namespace MessagingMesh;

Field::Field(const std::string& name) :
    m_name(name),
    m_dataType(NOT_SET)
{
}

Field::~Field()
{
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
        throw FieldException("Field is not a signed int32");
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
        throw FieldException("Field is not a double");
    }
    return m_data_double;
}

void Field::setMessage(const MessagePtr& value)
{
    m_dataType = MESSAGE;
    m_data_message = value;
}

Field::MessageConstPtr Field::getMessage() const
{
    if (m_dataType != MESSAGE)
    {
        throw FieldException("Field is not a message");
    }
    return m_data_message;
}

