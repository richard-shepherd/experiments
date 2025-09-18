#pragma once
#include <vector>
#include <map>
#include <string>
#include <functional>
#include "Field.h"

namespace MessagingMesh
{
    // A message which can be sent via the Messaging Mesh.
    // Holds a vector of named Fields which can be accessed by
    // name or by index. You can add multiple fields with the
    // same name.
    class Message
    {
    // Shared pointer and object creation...
    public:
        // Shared pointer to a Message.
        typedef std::shared_ptr<Message> Ptr;

        // Shared pointer to a const Message.
        typedef std::shared_ptr<const Message> ConstPtr;

        // Creates a Message instance.
        static Ptr create() { return Ptr(new Message()); }

    // Public methods...
    public:
        // Destructor.
        ~Message();

        // Adds a signed int32 field to the message. 
        void addField(const std::string& name, int32_t value);

        // Adds a double field to the message. 
        void addField(const std::string& name, double value);

        // Adds a message field to the message. 
        void addField(const std::string& name, const Message::Ptr& value);
    
    // Private functions...
    private:
        // Constructor.
        // NOTE: The constructor is private. Use Message::create() to create an instance.
        Message();

        // Adds a field to the message, setting its value using the valueSetter function (lambda)...
        void addField(const std::string& name, std::function<void(const Field::Ptr&)> valueSetter);

    // Private data...
    private:
        // Vector of fields in the message, in the order they were added...
        std::vector<Field::Ptr> m_fields;

        // Map of field name to the first field with that name...
        std::map<std::string, Field::Ptr> m_mapNameToField;

    };

} // namespace

