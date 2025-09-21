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
        auto person = Message::create();
        person->addField("NAME", "richard");
        person->addField("AGE", 56);
        auto address = Message::create();
        address->addField("HOUSE_NUMBER", 3);
        address->addField("POST_CODE", "IM4 3LT");
        person->addField("ADDRESS", address);

        auto buffer = Buffer::create();
        buffer->write(person);

    }
    catch (const std::exception& ex)
    {
        std::cout << "Exception: " << ex.what() << std::endl;
    }
}

