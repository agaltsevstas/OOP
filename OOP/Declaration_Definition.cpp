#include "Declaration_Definition.hpp"


namespace internal_linkage
{
    namespace
    {
        [[maybe_unused]] int number = 10; // В main.cpp есть такая же переменная с таким же названием
    }
    
    namespace print
    {
        int& Number1() { return number1; }
        const int& Number2() { return number2; }
        const int& Number3() { return number3; }
        const int& Number4() { return number4; }
        const int& Number5() { return number5; }
        int& Number6() { return number6; }
        ptr Static_Function() { return &static_function; }
    }
}

namespace external_linkage
{
    int number1 = 10;

    // В main.cpp есть такая стуктура с таким же названием
    struct Example
    {
        // В main.cpp есть static method с таким же названием
        static void print()
        {
            std::cout << "Declaration_Definition.cpp" << std::endl;
        }
    };

    void Print_Example()
    {
        Example::print(); // UB: По идее должен вывести: "Declaration_Definition.cpp", но выведет "main.cpp"
    }

    int Example2::number1 = 1;
    const int Example2::number2 = 1;

    namespace print
    {
        int& Number1() { return number1; }
        const int& Number2() { return number2; }
        const int& Number3() { return number3; }
        ptr Static_Function() { return &Example2::static_function; }
    }
}

namespace declaration_definition // Определение
{
    void Print()
    {
        std::cout << "Hello World!" << std::endl;
    }

    inline void Print_number() // Нарушение ODR (UB): результат не определён, в каждой единице трансляции (.cpp) функция должна быть объявлена одинаково и должно быть ровно одно определение.
    {
        std::cout << "number" << std::endl;
    }
}
