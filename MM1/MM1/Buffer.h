#pragma once
#include <memory>
#include <vector>

namespace MessagingMesh
{
    // A binary buffer managing a vector of bytes (unsigned char), holding
    // for example the network serialization of a NetworkMessage.
    class Buffer
    {
    // Shared pointer and object creation...
    public:
        // Shared pointer to a Buffer.
        typedef std::shared_ptr<Buffer> Ptr;

        // Shared pointer to a const Buffer.
        typedef std::shared_ptr<const Buffer> ConstPtr;

        // Creates a Buffer instance.
        static Ptr create() { return Ptr(new Buffer()); }
        
    // Public methods...
    public:
        // Destructor.
        ~Buffer();

    // Private functions...
    private:
        // Constructor.
        // NOTE: The constructor is private. Use Buffer::create() to create an instance.
        Buffer();

    // Private data...
    private:
        // The initial size of the vector...
        const unsigned int INITIAL_SIZE = 8192;

        // The vector of bytes. Starts at INITIAL_SIZE and is doubled (repeatedly
        // if necessary) if data written to the buffer would overrun the size of
        // the vector...
        std::vector<unsigned char> m_buffer;
    };

} // namespace
