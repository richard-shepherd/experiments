#include <iostream>
#include "Logger.h"
#include "Gateway.h"
#include "Utils.h"
#include "Tests.h"
#include "Connection.h"
#include "Message.h"
using namespace MessagingMesh;


void onMessageLogged(Logger::LogLevel logLevel, const std::string& message)
{
    auto time = Utils::getTimeString();
    std::cout << time << ": " << Logger::toString(logLevel) << ": " << message << std::endl;
}

void runClient()
{
    // We connect to the gateway...
    Connection connection("localhost", 5050, "VULCAN");

    Logger::info("Sending data");
    for (int32_t i = 1; i <= 5; ++i)
    {
        auto pMessage = Message::create();
        pMessage->addField("VALUE", i);
        connection.sendMessage("A.B", pMessage);
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

