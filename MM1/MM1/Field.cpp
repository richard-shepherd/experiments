#include "Field.h"
#include "MessagingMeshExceptions.h"
using namespace MessagingMesh;

/// <summary>
/// Constructor.
/// </summary>
Field::Field() :
    m_dataType(NOT_SET),
    m_data({ 0 })
{
}

/// <summary>
/// Destructor.
/// </summary>
Field::~Field()
{
}

/// <summary>
/// Sets the field to hold a signed int32.
/// </summary>
void Field::setSignedInt32(int value)
{
    m_dataType = SIGNED_INT32;
    m_data.data_int = value;
}

/// <summary>
/// Returns the signed integer held by the field.
/// Throws a FieldException if the field does not hold this type.
/// </summary>
int Field::getSignedInt32() const
{
    if (m_dataType != SIGNED_INT32)
    {
        throw FieldException("Field is not a signed int32");
    }
    return m_data.data_int;
}

/// <summary>
/// Sets the field to hold a double.
/// </summary>
void Field::setDouble(double value)
{
    m_dataType = DOUBLE;
    m_data.data_double = value;
}

/// <summary>
/// Returns the double held by the field.
/// Throws a FieldException if the field does not hold this type.
/// </summary>
double Field::getDouble() const
{
    if (m_dataType != DOUBLE)
    {
        throw FieldException("Field is not a double");
    }
    return m_data.data_double;
}
