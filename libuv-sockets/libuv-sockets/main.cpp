#include <iostream>
#include "Logger.h"
#include "Gateway.h"
#include "UVUtils.h"
using namespace MessagingMesh;


void onMessageLogged(Logger::LogLevel logLevel, const std::string& message)
{
    std::cout << "MessagingMesh: " + Logger::toString(logLevel) + ": " + message << std::endl;
}

void runClient()
{
    SocketPtr clientSocket;
    std::unique_ptr<UVLoopThread> clientUVLoop = std::make_unique<UVLoopThread>("CLIENT");
    clientUVLoop->marshallEvent(
        [&clientSocket](uv_loop_t* pLoop)
        {
            clientSocket = Socket::create(pLoop);
            clientSocket->connectIP("127.0.0.1", 5050);
        }
    );

    Logger::info("Press Enter to exit");
    std::cin.get();
}

void runServer()
{
    std::unique_ptr<Gateway> gateway = std::make_unique<Gateway>(5050);

    Logger::info("Press Enter to exit");
    std::cin.get();
}

int main(int argc, char* argv[])
{
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

