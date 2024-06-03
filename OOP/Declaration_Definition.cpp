#include "Declaration_Definition.hpp"

#include <iostream>


namespace internal_linkage
{
    void print()
    {
        std::cout << "static number1: " << &number1 << std::endl;
        std::cout << "const number2: " << &number2 << std::endl;
        std::cout << "constexpr number3: " << &number3 << std::endl;
    }
}

namespace external_linkage
{
    int number1 = 10;
    int Number::number = 10;
    void print()
    {
        std::cout << "extern number1: " << &number1 << std::endl;
        std::cout << "inline number2: " << &number2 << std::endl;
        std::cout << "static constexpr number: " << &Number::number << std::endl;
    }
}

namespace declaration_definition // Определение
{
    void print()
    {
        std::cout << "Hello World!" << std::endl;
    }

    inline void print_number() // в каждой единице трансляции (.cpp) должен быть свой inline
    {
        std::cout << "number" << std::endl;
    }
}
