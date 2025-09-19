#include <iostream>
#include <vector>
#include <map>
#include "Message.h"
using namespace MessagingMesh;


int main()
{
    try
    {
        int a = 12;
        std::vector<unsigned char> vbytes;
        vbytes.resize(4);
        std::cout << vbytes.size() << std::endl;
        memcpy(&vbytes[0], &a, 4);
        vbytes.resize(10);
        std::cout << vbytes.size() << std::endl;

        auto m = Message::create();
        m->addField("hello", 123);
        m->addField("hello", 234.567);
        m->addField("world", 17);
        m->addField("sub-message", Message::create());

        auto f = Field::create();
        f->setName("Count");
        f->setSignedInt32(32);
        std::cout << f->getDouble() << std::endl;
    }
    catch (const std::exception& ex)
    {
        std::cout << "Exception: " << ex.what() << std::endl;
    }
}

