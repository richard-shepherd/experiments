#pragma once
#include <string>

namespace MessagingMesh
{
    /// <summary>
    /// Static utility functions for messaging mesh.
    /// </summary>
    class Utils
    {
    // Public functions...
    public:
        // Returns a std::string created using the string format and variadic arguments.
        static std::string format(const char* format, ...);

        // Returns a time string in the format HH:MM:SS.mmm
        static std::string getTimeString();
    };

} // namespace


