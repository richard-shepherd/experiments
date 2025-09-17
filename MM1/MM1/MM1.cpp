#include <iostream>
#include "Field.h"

int main()
{
    try
    {
        MessagingMesh::Field field;
        field.setDouble(123.45);
        std::cout << field.getDouble();
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what();
    }
}

