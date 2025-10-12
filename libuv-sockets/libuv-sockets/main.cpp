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
    auto clientUVLoop = std::make_unique<UVLoopThread>("CLIENT");

    // We connect to the server...
    clientUVLoop->marshallEvent(
        [&clientSocket](uv_loop_t* pLoop)
        {
            clientSocket = Socket::create(pLoop);
            clientSocket->connectIP("127.0.0.1", 5050);
        }
    );

    // RSSTODO: We need to wait for the socket to connect
    uv_sleep(1000);

    // We send some data...
    for (auto i = 0; i < 10; ++i)
    {
        clientUVLoop->marshallEvent(
            [&clientSocket, i](uv_loop_t* pLoop)
            {
                int data[] = { 4, i };
                auto pBuffer = UVUtils::createBuffer(8);
                memcpy(pBuffer->base, &data[0], sizeof(data));
                clientSocket->write(pBuffer);
            }
        );
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

