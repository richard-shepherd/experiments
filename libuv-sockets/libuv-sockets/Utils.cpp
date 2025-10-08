#include <cstdarg>
#include "Utils.h"
using namespace MessagingMesh;

// Returns a std::string created using the string format and variadic arguments.
std::string Utils::format(const char* format, ...)
{
    // We allocate a result string with an initial size...
    const int BUFFER_SIZE = 4096;
    std::string result;
    result.resize(BUFFER_SIZE);

    // We format the data into the result...
    va_list args;
    va_start(args, format);
    int size = vsnprintf(&result[0], BUFFER_SIZE + 1, format, args);
    va_end(args);

    // If the formatted string has fit into the result, we return it...
    if (size <= BUFFER_SIZE)
    {
        return result;
    }

    // The formatted string did not fit, so we resize it and format it into the new size...
    result.resize(size);
    va_start(args, format);
    vsnprintf(&result[0], size + 1, format, args);
    va_end(args);

    return result;
}
