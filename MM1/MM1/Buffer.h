#pragma once
#include <memory>
#include <vector>
#include "SharedPointers.h"

namespace MessagingMesh
{
    // A binary buffer managing a vector of bytes (unsigned char), holding
    // for example the network serialization of a NetworkMessage.
    //
    // The buffer starts at an initial size which will be expanded if data 
    // is written to the buffer that would go beyond that size.
    //
    // Data is written to the buffer at the current position. By default this
    // will be updated to the next available position as data is written to the 
    // buffer. You can change the position manually if you need to write into 
    // the buffer at a point other than the end.
    //
    // You can request the data size from the buffer which will return the number 
    // of bytes in the buffer, regardless of where the position is set.
    class Buffer
    {
    // Public methods...
    public:
        // Creates a Buffer instance.
        static BufferPtr create() { return BufferPtr(new Buffer()); }

        // Destructor.
        ~Buffer();

        // void clear(); TODO does not reset the size of the buffer (create a new instance to do this)

        // Gets the position in the buffer where data will be written.
        size_t getPosition() const { return m_position; }

        // Sets the position in the buffer where data will be written.
        void setPosition(size_t position) { m_position = position; }

        // Adds an unsigned char to the buffer.
        void add(unsigned char item);

        // Adds a serialized field to the buffer.
        void add(const ConstFieldPtr& item);

    // Private functions...
    private:
        // Constructor.
        // NOTE: The constructor is private. Use Buffer::create() to create an instance.
        Buffer();

        // Checks that the buffer has the capacity to hold the number of bytes specified
        // and expands it if it does not...
        void checkBufferSize(size_t bytesRequired);

        // Updates the position and data-size to reflect the bytes written...
        void updatePosition(size_t bytesWritten);

    // Private data...
    private:
        // The initial size of the vector...
        const size_t INITIAL_SIZE = 1;

        // The vector of bytes...
        std::vector<unsigned char> m_data;

        // The current position...
        size_t m_position = 0;

        // The size of all data written to the buffer...
        size_t m_dataSize = 0;
    };

} // namespace
