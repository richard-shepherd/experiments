#include <iostream>
#include "Logger.h"
#include "Gateway.h"
using namespace MessagingMesh;


void onMessageLogged(Logger::LogLevel logLevel, const std::string& message)
{
    std::cout << "MessagingMesh: " + Logger::toString(logLevel) + ": " + message << std::endl;
}

int main()
{
    Logger::registerCallback(onMessageLogged);

    Gateway gateway(5050);
    uv_sleep(1000);
}

