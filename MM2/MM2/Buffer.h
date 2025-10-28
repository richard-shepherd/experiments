#pragma once
#include <memory>
#include <vector>
#include <string>
#include "SharedPointers.h"

namespace MessagingMesh
{
    /// <summary>
    /// A binary buffer managing a vector of bytes (unsigned char), holding
    /// for example the network serialization of a NetworkMessage.
    ///
    /// The buffer starts at an initial size which will be expanded if data 
    /// is written to the buffer that would go beyond that size.
    ///
    /// Data is written to the buffer at the current position. By default this
    /// will be updated to the next available position as data is written to the 
    /// buffer. You can change the position manually if you need to write into 
    /// the buffer at a point other than the end.
    ///
    /// You can request the data size from the buffer which will return the number 
    /// of bytes in the buffer, regardless of where the position is set.
    ///
    /// The buffer includes the size
    /// ----------------------------
    /// The first four bytes of the buffer is the buffer size stored in a little-endian
    /// format. This helps when sending buffers over the network as receiving code can
    /// see how much data to expect.
    /// 
    /// The size is added to the buffer when the getBuffer() method is called.
    /// </summary>
    class Buffer
    {
    // Public methods...
    public:
        // Creates a Buffer instance.
        static BufferPtr create() { return BufferPtr(new Buffer()); }

        // Destructor.
        ~Buffer();

        // Resets the position to the initial position for reading data.
        // Note: This is the position after the size.
        void resetPosition() { m_position = INITIAL_POSITION; }

        // Gets the position in the buffer where data will be written.
        int32_t getPosition() const { return m_position; }

        // Sets the position in the buffer where data will be written.
        void setPosition(int32_t position) { m_position = position; }

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
        void write(const void* p, int32_t size);

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
        void read(void* p, int32_t size);

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

        // Expands the buffer by doubling its size.
        void expandBuffer();

        // Updates the position to reflect the bytes read.
        void updatePosition_Read(int32_t bytesWritten);

        // Updates the position and data-size to reflect the bytes written.
        void updatePosition_Write(int32_t bytesWritten);

    // Private data...
    private:
        // The initial size of the vector...
        const int32_t INITIAL_SIZE = 8192;

        // We reserve the first four bytes of the buffer for the size.
        // (See heading comment.)
        const int32_t INITIAL_POSITION = 4;

        // The buffer...
        char* m_pBuffer;
        int32_t m_bufferSize;

        // The current position...
        int32_t m_position = INITIAL_POSITION;

        // The size of all data written to the buffer...
        int32_t m_dataSize = 0;
    };

} // namespace
