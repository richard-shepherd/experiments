#pragma once
#include <memory>
#include <string>
#include "Exception.h"
#include "SharedPointers.h"

// Checks that the data-type we hold is an the type specified. TODO: Is using a macro (for enum->string) too horrible?
#define CHECK_DATA_TYPE(x) if(m_dataType != x) throw Exception("Field '" + m_name + "' is not a " + #x)

namespace MessagingMesh
{
    // Holds one field in a message.
    class Field
    {
    // Public types...
    public:
        enum DataType
        {
            NOT_SET,
            STRING,
            SIGNED_INT32,
            DOUBLE,
            MESSAGE
        };
        
    // Public methods...
    public:
        // Creates a Field instance.
        static FieldPtr create() { return FieldPtr(new Field()); }

        // Destructor.
        ~Field();

        // Sets the field's name.
        void setName(const std::string& name);

        // Gets the field's name.
        const std::string& getName() const;

        // Serializes the field to the current position of the buffer.
        void serialize(Buffer& buffer) const;

    // Getters and setters for field types...
    public:
        // Gets the string held by the field.
        // Throws a MessagingMesh::Exception if the field does not hold this type.
        const std::string& getString() const { CHECK_DATA_TYPE(STRING); return m_data_string; }

        // Sets the field to hold a string.
        void setString(const std::string& value) { m_dataType = STRING; m_data_string = value; }

        // Gets the signed int32 held by the field.
        // Throws a MessagingMesh::Exception if the field does not hold this type.
        int32_t getSignedInt32() const { CHECK_DATA_TYPE(SIGNED_INT32); return m_data_int32; }

        // Sets the field to hold a signed int32.
        void setSignedInt32(int32_t value) { m_dataType = SIGNED_INT32; m_data_int32 = value; }

        // Gets the double held by the field.
        // Throws a MessagingMesh::Exception  if the field does not hold this type.
        double getDouble() const { CHECK_DATA_TYPE(DOUBLE); return m_data_double; }

        // Sets the field to hold a double.
        void setDouble(double value) { m_dataType = DOUBLE; m_data_double = value; }

        // Gets the message held by the field.
        // Throws a MessagingMesh::Exception  if the field does not hold this type.
        const ConstMessagePtr& getMessage() const { CHECK_DATA_TYPE(MESSAGE); return m_data_message; }

        // Sets the field to hold a message.
        void setMessage(const ConstMessagePtr& value) { m_dataType = MESSAGE; m_data_message = value; }
        
    // Private functions...
    private:
        // Constructor.
        // NOTE: The constructor is private. Use Field::create() to create an instance.
        Field();

    // Private data...
    private:
        std::string m_name;
        DataType m_dataType;

        // Data for the various supported types...
        std::string m_data_string;
        int32_t m_data_int32 = 0;
        double m_data_double = 0.0;
        ConstMessagePtr m_data_message = nullptr;

    };

} // namespace

