#pragma once
#include <vector>
#include <map>
#include <string>
#include <functional>
#include "SharedPointers.h"

namespace MessagingMesh
{
    // A message which can be sent via the Messaging Mesh.
    // Holds a vector of named Fields which can be accessed by
    // name or by index. You can add multiple fields with the
    // same name.
    class Message
    {
    // Public methods...
    public:
        // Creates a Message instance.
        static MessagePtr create() { return MessagePtr(new Message()); }

        // Destructor.
        ~Message();

        // Adds a signed int32 field to the message. 
        void addField(const std::string& name, int32_t value);

        // Adds a double field to the message. 
        void addField(const std::string& name, double value);

        // Adds a message field to the message. 
        void addField(const std::string& name, const MessagePtr& value);
    
    // Private functions...
    private:
        // Constructor.
        // NOTE: The constructor is private. Use Message::create() to create an instance.
        Message();

        // Adds a field to the message, setting its value using the valueSetter function (lambda).
        void addField(const std::string& name, std::function<void(const FieldPtr&)> valueSetter);

    // Private data...
    private:
        // Vector of fields in the message, in the order they were added...
        std::vector<FieldPtr> m_fields;

        // Map of field name to the first field with that name...
        std::map<std::string, FieldPtr> m_mapNameToField;

    };

} // namespace

