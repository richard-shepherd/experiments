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

void Field::serialize(Buffer& buffer) const
{
    // We serialize the field name...
    buffer.write(m_name);

    // We serialize the data type...
    buffer.write(static_cast<unsigned char>(m_dataType));

    // We serialize the data...
    switch (m_dataType)
    {
    case STRING:
        buffer.write(m_data_string);
        break;

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
