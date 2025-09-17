#pragma once
#include <exception>
#include <string>

namespace MessagingMesh
{
    /// <summary>
    /// Base messaging mesh exception.
    /// </summary>
    class Exception : public std::exception
    {
    public:
        Exception(const std::string& message)
            : std::exception(message.c_str())
        {
        }
    };

    /// <summary>
    /// Exceptions thrown by invalid interactions with the Field class.
    /// </summary>
    class FieldException : public Exception
    {
    public:
        FieldException(const std::string& message)
            : Exception(message)
        {
        }
    };
} // namespace

