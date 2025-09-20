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
        auto f1 = Field::create();
        f1->setName("AGE");
        f1->setSignedInt32(56);
        auto buffer = Buffer::create();
        buffer->write(f1);


        //auto m = Message::create();
        //m->addField("hello", 123);
        //m->addField("hello", 234.567);
        //m->addField("world", 17);
        //m->addField("sub-message", Message::create());

        //auto f = Field::create();
        //f->setName("Count");
        //f->setSignedInt32(32);
        //std::cout << f->getDouble() << std::endl;
    }
    catch (const std::exception& ex)
    {
        std::cout << "Exception: " << ex.what() << std::endl;
    }
}

