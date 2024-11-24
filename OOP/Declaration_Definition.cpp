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

    namespace
    {
        // В main.cpp есть такая стуктура с таким же названием
        struct Example
        {
            // В main.cpp есть static method с таким же названием
            static void print()
            {
                std::cout << "Declaration_Definition.cpp" << std::endl;
            }
        };
    }

    void print_Example()
    {
        Example::print(); // UB: По идее должен вывести: "Declaration_Definition.cpp", но выведет "main.cpp"
    }
}

namespace external_linkage
{
    int number1 = 10;
    void print()
    {
        std::cout << "extern number1: " << &number1 << std::endl;
        std::cout << "inline number2: " << &number2 << std::endl;
    }

    // В main.cpp есть такая стуктура с таким же названием
    struct Example1
    {
        // В main.cpp есть static method с таким же названием
        static void print()
        {
            std::cout << "Declaration_Definition.cpp" << std::endl;
        }
    };

    void print_Example()
    {
        Example1::print(); // UB: По идее должен вывести: "Declaration_Definition.cpp", но выведет "main.cpp"
    }

    int Example2::number1 = 1;
    const int Example2::number2 = 1;
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
