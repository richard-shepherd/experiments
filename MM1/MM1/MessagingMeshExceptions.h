#pragma once
#include <exception>
#include <string>

namespace MessagingMesh
{
    // Base messaging mesh exception.
    class Exception : public std::exception
    {
    public:
        Exception(const std::string& message)
            : std::exception(message.c_str())
        {
        }
    };

    // Exceptions thrown by invalid interactions with the Field class.
    class FieldException : public Exception
    {
    public:
        FieldException(const std::string& message)
            : Exception(message)
        {
        }
    };

    // Exceptions thrown by invalid interactions with the Message class.
    class MessageException : public Exception
    {
    public:
        MessageException(const std::string& message)
            : Exception(message)
        {
        }
    };

} // namespace

