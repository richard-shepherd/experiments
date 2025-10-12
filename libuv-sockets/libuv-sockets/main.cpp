#include <iostream>
#include "Logger.h"
#include "Gateway.h"
#include "UVUtils.h"
using namespace MessagingMesh;


void onMessageLogged(Logger::LogLevel logLevel, const std::string& message)
{
    std::cout << "MessagingMesh: " + Logger::toString(logLevel) + ": " + message << std::endl;
}

int main()
{
    Logger::registerCallback(onMessageLogged);

    char data[] = {4, 0, 0, 0, 1, 2, 3, 4};

    uv_buf_t buffer;
    UVUtils::allocateBuffer(nullptr, sizeof(data), &buffer);
    memcpy(buffer.base, &data[0], sizeof(data));
    buffer.len = sizeof(data);
    auto s = Socket::create(nullptr);
    s->onDataReceived(nullptr, sizeof(data), &buffer);

    //Gateway gateway(5050);
    //uv_sleep(1000);
}

