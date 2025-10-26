#pragma once
#include <memory>
#include <vector>
#include <string>
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
    // Public types...
    public:
        // Typedef for the vecotr of data held in the buffer.
        typedef std::vector<unsigned char> VecData;

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

        // Gets the data.
        // NOTE: The vector may be larger than the data stored in it. Use getDataSize()
        //       to get the data size.
        const VecData& getData() const { return m_data; }

        // Gets the size of data stored in the buffer.
        size_t getDataSize() const { return m_dataSize; }

    // write() method for various types...
    public:
        // Writes an int8 to the buffer.
        void write(int8_t item);

        // Writes a signed int32 to the buffer.
        void write(int32_t item);

        // Writes an unsigned int32 to the buffer.
        void write(uint32_t item);

        // Writes a double to the buffer.
        void write(double item);

        // Writes a string to the buffer.
        void write(const std::string& item);

        // Writes bytes to the buffer from the pointer passed in.
        void write(const void* p, size_t size);

        // Writes a field to the buffer.
        void write(const ConstFieldPtr& item);

        // Writes a message to the buffer.
        void write(const ConstMessagePtr& item);

    // read() method for various types...
    public:
        // Reads an int8 from the buffer.
        int8_t readInt8();

        // Reads an int32 from the buffer.
        int32_t readInt32();

        // Reads a uint32 from the buffer.
        uint32_t readUInt32();

        // Reads a double from the buffer.
        double readDouble();

        // Reads a string from the buffer.
        std::string readString();

        // Reads bytes from the buffer to the pointer passed in.
        // NOTE: You must make sure that the memory pointed to is large enough.
        void read(void* p, size_t size);

        // Reads a field from the buffer.
        ConstFieldPtr readField();

        // Reads a message from the buffer.
        ConstMessagePtr readMessage();

    // Private functions...
    private:
        // Constructor.
        // NOTE: The constructor is private. Use Buffer::create() to create an instance.
        Buffer();

        // Writes an item to the buffer which can be written with memcpy.
        template <typename T> void writeCopyable(const T& item);

        // Reads an item from the buffer using memcpy.
        template <typename T> void readCopyable(T& item);

        // Checks that the buffer has the capacity to hold the number of bytes specified
        // and expands it if it does not.
        // Throws a MessagingMesh::Exception if the buffer required is too large.
        void checkBufferSize_Write(size_t bytesRequired);

        // Checks that the buffer is large enough to read the specified number of bytes.
        // Throws a MessagingMesh::Exception if the buffer is not large enough.
        void checkBufferSize_Read(size_t bytesRequired);

        // Updates the position to reflect the bytes read.
        void updatePosition_Read(size_t bytesWritten);

        // Updates the position and data-size to reflect the bytes written.
        void updatePosition_Write(size_t bytesWritten);

    // Private data...
    private:
        // The initial size of the vector...
        const size_t INITIAL_SIZE = 8192;

        // The vector of bytes...
        VecData m_data;

        // The current position...
        size_t m_position = 0;

        // The size of all data written to the buffer...
        size_t m_dataSize = 0;
    };

} // namespace
