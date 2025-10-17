#include <iostream>
#include <ctime>
#include "Logger.h"
#include "Gateway.h"
#include "NetworkData.h"
#include "Utils.h"
using namespace MessagingMesh;


void onMessageLogged(Logger::LogLevel logLevel, const std::string& message)
{
    std::cout << Utils::getTimeString() << ": " << "MessagingMesh: " + Logger::toString(logLevel) + ": " + message << std::endl;
}

void runClient()
{
    UVLoop clientUVLoop("CLIENT");
    SocketPtr clientSocket;

    // We connect to the server...
    clientUVLoop.marshallEvent(
        [&clientSocket, &clientUVLoop](uv_loop_t* pLoop)
        {
            clientSocket = Socket::create(clientUVLoop);
            clientSocket->connectIP("127.0.0.1", 5050);
        }
    );

    // RSSTODO: We need to wait for the socket to connect
    uv_sleep(1000);

    // We send some data...
    Logger::info("Sending data");
    const int size = 10;
    for (auto i = 0; i < 5000000; ++i)
    {
        int data[size];
        data[0] = size * 4 - 4;
        auto pNetworkData = NetworkData::create(&data[0], sizeof(data));
        clientSocket->queueWrite(pNetworkData);
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

