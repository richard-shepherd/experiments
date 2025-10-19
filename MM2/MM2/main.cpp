#include <iostream>
#include <ctime>
#include <thread>
#include "Logger.h"
#include "Gateway.h"
#include "NetworkData.h"
#include "Utils.h"
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
        [pSocket](uv_loop_t* pLoop)
        {
            pSocket->connectIP("127.0.0.1", 5050);
        }
    );

    // We send some data...
    Logger::info("Sending data");
    const int size = 10;
    for (auto i = 0; i < 50000000; ++i)
    {
        int data[size + 1];
        data[0] = size * 4;
        auto pNetworkData = NetworkData::create(&data[0], sizeof(data));
        pSocket->write(pNetworkData);
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

int main(int argc, char* argv[])
{
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
        Logger::info("Usage: libuv-test1.exe -client / -server");
    }
}

