#pragma once
#include <memory>
#include <string>

namespace MessagingMesh
{
    // Holds one field in a message.
    class Field
    {
    // Shared pointer and object creation...
    public:
        // Shared pointer to a Field.
        typedef std::shared_ptr<Field> Ptr;

        // Shared pointer to a const Field.
        typedef std::shared_ptr<const Field> ConstPtr;

        // Creates a Field instance.
        static Ptr create(const std::string& name) { return Ptr(new Field(name)); }

    // Public methods...
    public:
        // Destructor.
        ~Field();

        // Gets the field's name.
        const std::string& getName() const;

        // Sets the field to hold a signed int32.
        void setSignedInt32(int32_t value);

        // Gets the signed int32 held by the field.
        // Throws a FieldException if the field does not hold this type.
        int32_t getSignedInt32() const;

        // Sets the field to hold a double.
        void setDouble(double value);

        // Gets the double held by the field.
        // Throws a FieldException if the field does not hold this type.
        double getDouble() const;

    // Private functions...
    private:
        // Constructor.
        // NOTE: The constructor is private. Use Field::create() to create an instance.
        Field(const std::string& name);

    // Private types...
    private:
        enum DataType
        {
            NOT_SET,
            SIGNED_INT32,
            DOUBLE
        };

        union DataUnion
        {
            int32_t data_int32;
            double data_double;
        };

    // Private data...
    private:
        std::string m_name;
        DataType m_dataType;
        DataUnion m_data;
    };

} // namespace

