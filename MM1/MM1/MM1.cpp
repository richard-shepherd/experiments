#include <iostream>
#include <vector>
#include <map>
#include "Message.h"
using namespace MessagingMesh;


int main()
{
    try
    {
        auto m = Message::create();
        m->addField("hello", 123);
        m->addField("hello", 234.567);
        m->addField("world", 17);
        m->addField("sub-message", Message::create());
    }
    catch (const std::exception& ex)
    {
        std::cout << "Exception: " << ex.what() << std::endl;
    }
}

