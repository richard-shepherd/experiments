#include "Field.h"
#include "MessagingMeshExceptions.h"
using namespace MessagingMesh;

Field::Field(const std::string& name) :
    m_name(name),
    m_dataType(NOT_SET),
    m_data({ 0 })
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
    m_data.data_int32 = value;
}

int32_t Field::getSignedInt32() const
{
    if (m_dataType != SIGNED_INT32)
    {
        throw FieldException("Field is not a signed int32");
    }
    return m_data.data_int32;
}

void Field::setDouble(double value)
{
    m_dataType = DOUBLE;
    m_data.data_double = value;
}

double Field::getDouble() const
{
    if (m_dataType != DOUBLE)
    {
        throw FieldException("Field is not a double");
    }
    return m_data.data_double;
}
