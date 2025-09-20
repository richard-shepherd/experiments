#pragma once
#include <memory>

namespace MessagingMesh
{
    // Forward declarations...
    class Field;
    class Message;
    class Buffer;

    // Shared pointer to a Field.
    typedef std::shared_ptr<Field> FieldPtr;

    // Shared pointer to a const Field.
    typedef std::shared_ptr<const Field> ConstFieldPtr;

    // Shared pointer to a Message.
    typedef std::shared_ptr<Message> MessagePtr;

    // Shared pointer to a const Message.
    typedef std::shared_ptr<const Message> ConstMessagePtr;

    // Shared pointer to a Buffer.
    typedef std::shared_ptr<Buffer> BufferPtr;

    // Shared pointer to a const Buffer.
    typedef std::shared_ptr<const Buffer> ConstBufferPtr;

}// namespace
