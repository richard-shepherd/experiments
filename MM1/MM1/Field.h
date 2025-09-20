#pragma once
#include <memory>
#include <string>
#include "SharedPointers.h"

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
        // Gets the signed int32 held by the field.
        // Throws a MessagingMesh::Exception if the field does not hold this type.
        int32_t getSignedInt32() const;

        // Sets the field to hold a signed int32.
        void setSignedInt32(int32_t value);

        // Gets the double held by the field.
        // Throws a MessagingMesh::Exception  if the field does not hold this type.
        double getDouble() const;

        // Sets the field to hold a double.
        void setDouble(double value);

        // Gets the message held by the field.
        // Throws a MessagingMesh::Exception  if the field does not hold this type.
        ConstMessagePtr getMessage() const;

        // Sets the field to hold a message.
        void setMessage(const MessagePtr& value);
        
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
        int32_t m_data_int32 = 0;
        double m_data_double = 0.0;
        MessagePtr m_data_message = nullptr;

    };

} // namespace

