#include <iostream>
#include <ctime>
#include <thread>
#include "Logger.h"
#include "Gateway.h"
#include "Buffer.h"
#include "Utils.h"
#include "Tests.h"
using namespace MessagingMesh;


void onMessageLogged(Logger::LogLevel logLevel, const std::string& message)
{
    auto time = Utils::getTimeString();
    std::cout << time << ": " << Logger::toString(logLevel) << ": " << message << std::endl;
}

void runClient()
{
    auto pUVLoop = UVLoop::create("CLIENT");
    auto pSocket = Socket::create(pUVLoop);

    // We connect to the server...
    pUVLoop->marshallEvent(
        [pSocket](uv_loop_t* /*pLoop*/)
        {
            pSocket->connect("localhost", 5050);
        }
    );

    // We send some data...
    Logger::info("Sending data");
    for (auto i = 0; i < 50000000; ++i)
    {
        auto pBuffer = Buffer::create();
        pBuffer->write("Hello");
        pBuffer->write(123);
        pSocket->write(pBuffer);
        //if (i % 100000 == 0) uv_sleep(100);
    }

    Logger::info("Press Enter to exit");
    std::cin.get();
}

void runServer()
{
    auto gateway = std::make_unique<Gateway>(5050);

    Logger::info("Press Enter to exit");
    std::cin.get();
}

int main(int argc, char** argv)
{
    //Logger::registerCallback(onMessageLogged);
    //Tests::messageSerialization();
    //return 0;

    UVUtils::setThreadName("MAIN");
    Logger::registerCallback(onMessageLogged);

    const char CLIENT[] = "-client";
    const char SERVER[] = "-server";
    if (argc >= 2 && strncmp(argv[1], CLIENT, sizeof(CLIENT)) == 0)
    {
        runClient();
    }
    else if (argc >= 2 && strncmp(argv[1], SERVER, sizeof(SERVER)) == 0)
    {
        runServer();
    }
    else
    {
        Logger::info("Usage: MM2.exe -client / -server");
    }
}

