#include "Logger.h"
using namespace MessagingMesh;

// Static fields...
std::vector<Logger::Callback> Logger::m_callbacks;

// Log levels as strings...
const std::string Logger::DEBUG_STRING = "DEBUG";
const std::string Logger::INFO_STRING = "INFO";
const std::string Logger::WARN_STRING = "WARN";
const std::string Logger::ERROR_STRING = "ERROR";
const std::string Logger::FATAL_STRING = "FATAL";
const std::string Logger::UNKNOWN_LOG_LEVEL_STRING = "[UNKNOWN-LOG-LEVEL]";


// Registers a function to be called when messages are logged.
void Logger::registerCallback(Callback callback)
{
    m_callbacks.push_back(callback);
}

// Logs a message at the level specified.
void Logger::log(LogLevel logLevel, const std::string& message)
{
    for (auto& callback : m_callbacks)
    {
        callback(logLevel, message);
    }
}

// Logs a message at DEBUG level.
void Logger::debug(const std::string& message)
{
    log(LOG_LEVEL_DEBUG, message);
}

// Logs a message at INFO level.
void Logger::info(const std::string& message)
{
    log(LOG_LEVEL_INFO, message);
}

// Logs a message at WARN level.
void Logger::warn(const std::string& message)
{
    log(LOG_LEVEL_WARN, message);
}

// Logs a message at ERROR level.
void Logger::error(const std::string& message)
{
    log(LOG_LEVEL_ERROR, message);
}

// Logs a message at FATAL level.
void Logger::fatal(const std::string& message)
{
    log(LOG_LEVEL_FATAL, message);
}

// Returns the log-level as a string.
const std::string& Logger::toString(LogLevel logLevel)
{
    switch (logLevel)
    {
    case LOG_LEVEL_DEBUG:
        return DEBUG_STRING;
    case LOG_LEVEL_INFO:
        return INFO_STRING;
    case LOG_LEVEL_WARN:
        return WARN_STRING;
    case LOG_LEVEL_ERROR:
        return ERROR_STRING;
    case LOG_LEVEL_FATAL:
        return FATAL_STRING;
    default:
        return UNKNOWN_LOG_LEVEL_STRING;
    }
}
