#include <iostream>
#include <vector>
#include <map>
#include "Message.h"
#include "Field.h"
#include "Buffer.h"
using namespace MessagingMesh;


int main()
{
    try
    {
        //auto person = Message::create();
        //person->addField("NAME", "richard");
        //person->addField("AGE", 56);
        //auto address = Message::create();
        //address->addField("HOUSE_NUMBER", 3);
        //address->addField("POST_CODE", "IM4 3LT");
        //person->addField("ADDRESS", address);

        //auto buffer = Buffer::create();
        //buffer->write(person);

        auto buffer = Buffer::create();
        buffer->write(static_cast<unsigned char>('h'));
        buffer->write("Messaging Mesh");
        buffer->write(12345);
        buffer->write(234.567);
        buffer->setPosition(0);
        std::cout << buffer->readUnsignedChar() << std::endl;
        std::cout << buffer->readString() << std::endl;
        std::cout << buffer->readInt32() << std::endl;
        std::cout << buffer->readDouble() << std::endl;

    }
    catch (const std::exception& ex)
    {
        std::cout << "Exception: " << ex.what() << std::endl;
    }
}

