#pragma once

namespace MessagingMesh
{
    /// <summary>
    /// Holds one field in a message.
    /// </summary>
    class Field
    {
    // Public methods...
    public:
        Field();
        ~Field();

        // Sets the field to hold a signed int32.
        void setSignedInt32(int value);

        // Returns the signed integer held by the field.
        // Throws a FieldException if the field does not hold this type.
        int getSignedInt32() const;

        // Sets the field to hold a double.
        void setDouble(double value);

        // Returns the double held by the field.
        // Throws a FieldException if the field does not hold this type.
        double getDouble() const;

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
            int data_int;
            double data_double;
        };

    // Private data...
    private:
        DataType m_dataType;
        DataUnion m_data;
    };
} // namespace

