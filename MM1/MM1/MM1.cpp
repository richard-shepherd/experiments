#include <iostream>
#include <vector>
#include <map>
#include "Message.h"
using namespace MessagingMesh;

void add(std::map<std::string, Field::Ptr>& map, const Field::Ptr& field)
{
    auto& name = field->getName();
    map.insert({ name, field });
}

int main()
{
    try
    {
        auto f = Field::create("hello");
        {
            std::map<std::string, Field::Ptr> map;
            std::cout << f.use_count() << std::endl;

            add(map, f);
            std::cout << f.use_count() << std::endl;
        }
        std::cout << f.use_count() << std::endl;

    }
    catch (const std::exception& ex)
    {
        std::cout << "Exception: " << ex.what() << std::endl;
    }
}

