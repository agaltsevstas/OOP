#ifndef Declaration_Definition_h
#define Declaration_Definition_h

/*
 Лекция: https://www.youtube.com/watch?v=c7CasTJKw7o&list=PLlb7e2G7aSpTFea2FYxp7mFfbZW-xavhL&index=11&ab_channel=ComputerScienceCenter
 Сайты: https://habr.com/ru/companies/otus/articles/432834/
        https://habr.com/ru/companies/jugru/articles/506104/
 */


namespace internal_linkage
{
    /*
     int number = 5; // Это не скомпилируется, потому что в каждой единице трансляции будет свое определение переменной
     Нужно сделать const, constexpr, inline:
     */
    static int number1 = 10;
    const int number2 = 10;
    constexpr int number3 = 10;

    /*
     const char* error = "error"; buffer — это указатель на константу, но не является константным указателем.
     Нужно сделать его константным:
    */
    const char* const buffer = "buffer";

    void print();
}

namespace external_linkage
{
    extern int number1;
    inline int number2 = 10;

    void print();
    /*
     int sqrt(int x)
     {
         return x * x;
     }
     Это не скомпилируется, потому что в каждой единице трансляции будет свое определение функции.
     Нужно сделать constexpr:
     */
    constexpr int sqrt1(int x)
    {
        return x * x;
    }

    inline int sqrt2(int x)
    {
        return x * x;
    }

    struct Number
    {
        static int number; // неявно является встроенной переменной
    };
}

#endif /* Declaration_Definition_h */
