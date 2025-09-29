#include <iostream>
#include <vector>
#include <map>
#include "Sockets.h"
#include "Message.h"
#include "Field.h"
#include "Buffer.h"
#include "Logger.h"
#include "Utils.h"
using namespace MessagingMesh;


void onMessageLogged(Logger::LogLevel logLevel, const std::string& message)
{
    std::cout << "MessagingMesh: " + Logger::toString(logLevel) + ": " + message << std::endl;
}


int main()
{
    try
    {
        Logger::registerCallback(onMessageLogged);

        Sockets::initialize();

        // We create a person message...
        auto person = Message::create();
        person->addField("NAME", "richard");
        person->addField("AGE", 56);

        // We add an address sub-message...
        auto address = Message::create();
        address->addField("HOUSE_NUMBER", 3);
        address->addField("POST_CODE", "IM4 3LT");
        person->addField("ADDRESS", address);

        // We write the person to a buffer...
        auto buffer = Buffer::create();
        buffer->write(person);

        // We deserialize the buffer to a new person message...
        buffer->setPosition(0);
        auto person2 = buffer->readMessage();

        auto& name = person2->getField("NAME")->getString();;
        auto& postcode = person2->getField("ADDRESS")->getMessage()->getField("POST_CODE")->getString();
        std::cout << name + ": " + postcode << std::endl;
    }
    catch (const std::exception& ex)
    {
        std::cout << "Exception: " << ex.what() << std::endl;
    }
}

