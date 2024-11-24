#include "RVO&NRVO.hpp"

#include <iostream>

/*
 Сайты: https://habr.com/ru/companies/vk/articles/666330/
 */

class A
{
public:
    A() { std::cout << "constructor" << std::endl; }
    A(int number) { std::cout << "constructor int" << std::endl; }
    explicit A(double number) { std::cout << "constructor doule" << std::endl; }
    A(const A&) { std::cout << "copy constructor" << std::endl; }
    A(A&&) noexcept { std::cout << "move constructor" << std::endl; }
    ~A() { std::cout << "destructor" << std::endl; }
};

namespace RVO
{
    A function()
    {
        return A();
    }

    A function_int()
    {
        int number = 10;
        return number;
    }

    A function_double_explicit()
    {
        int number = 10;
        return number;
    }
}

namespace NRVO
{
    A function()
    {
        A a;
        return a;
    }

    A function_move()
    {
        A a;
        return std::move(a); // Так писать не стоит, иначе компилятор не сможет применить NRVO
    }
}

namespace NO_NRVO
{
    A function(A a)
    {
        return a;
    }

    A function_ref(const A& a)
    {
        return a;
    }
}

namespace RVO_NRVO
{
    void Start()
    {
        std::cout << "RVO & NRVO" << std::endl;
        
        /*
         RVO (Return Value Optimization) - оптимизация возвращаемого значения. Компилятор при создании и возвращения в точке вызова return временного значения (rvalue) оптимизирует код, не вызывая лишние конструкторы копирования/перемещения, а вместо этого сразу вызывает конструктор в точке вызова функции.
         NRVO (Named Return Value Optimization) - именнованная оптимизация возвращаемого значения. Компилятор при создании и возвращения локальной переменной (lvalue) оптимизирует код, не вызывая лишние конструкторы копирования/перемещения, а вместо этого сразу вызывает конструктор в точке вызова функции.
         До 17-го стандарта RVO/NRVO было рекомендацией для компилятора, а в C++17 стало обязательной оптимизацией и по умолчанию включена. Отключить её можно при помощи флага компиляции -fno-elide-constructors.
         Условия для оптимизации:
         - при NRVO возвращаться должен ТОЛЬКО локальный объект, а не ссылка на него или какая-то его часть.
         - при NRVO возвращаемый объект не должен быть volatile и нельзя использовать std::move.
         - при RVO возвращаемый объект структуры/класса не должен иметь explicit конструктор.
         */
        
        /*
         RVO (Return Value Optimization) - оптимизация возвращаемого значения. Компилятор при создании и возвращения в точке вызова return временного значения (rvalue) оптимизирует код, не вызывая лишние конструкторы копирования/перемещения, а вместо этого сразу вызывает конструктор в точке вызова функции.
         */
        {
            using namespace RVO;
            std::cout << "RVO" << std::endl;
            
            /// 1 Пример
            {
                std::cout << "1 Пример" << std::endl;
                [[maybe_unused]] auto result = function();
                
                /*
                 Выведет с RVO:
                 constructor
                 destructor
                 */
                 
                /*
                 Выведет без RVO:
                 constructor
                 constructor copy
                 destructor
                 constructor copy
                 destructor
                 destructor
                 */
            }
            std::cout << std::endl;
            /// 2 Пример
            {
                std::cout << "2 Пример" << std::endl;
                
                [[maybe_unused]] auto result = function_int();
                
                /*
                 Выведет с RVO:
                 constructor number
                 destructor
                 */
                 
                /*
                 Выведет без RVO:
                 constructor number
                 constructor copy
                 destructor
                 constructor copy
                 destructor
                 destructor
                 */
            }
            std::cout << std::endl;
            /// 2 Пример: для explicit не сработает
            {
                std::cout << "3 Пример: для explicit не сработает" << std::endl;
                
#if 0
                [[maybe_unused]] auto result = function_double_explicit()();
#endif
            }
        }
        std::cout << std::endl;
        /*
         NRVO (Named Return Value Optimization) - именнованная оптимизация возвращаемого значения. Компилятор при создании и возвращения локальной переменной (lvalue) оптимизирует код, не вызывая лишние конструкторы копирования/перемещения, а вместо этого сразу вызывает конструктор в точке вызова функции.
         */
        {
            /// NRVO
            {
                using namespace NRVO;
                
                std::cout << "NRVO" << std::endl;
                
                /// 1 Пример
                {
                    std::cout << "1 Пример" << std::endl;
                    
                    [[maybe_unused]] auto result = function();
                    
                    /*
                     Выведет с NRVO:
                     constructor
                     destructor
                     */
                     
                    /*
                     Выведет без NRVO:
                     constructor
                     constructor copy
                     destructor
                     destructor
                     */
                }
                std::cout << std::endl;
                /// 2 Пример: использовать std::move - нет стоит, иначе компилятор не сможет применить NRVO. В таком случае в операторе return будет вызван перемещающий конструктор класса C
                {
                    std::cout << "2 Пример: использовать std::move - нет стоит, иначе компилятор не сможет применить NRVO" << std::endl;
                    
                    [[maybe_unused]] auto result = function_move();
                    
                    /*
                     Выведет без NRVO:
                     constructor
                     constructor move
                     destructor
                     destructor
                     */
                    
                    /*
                     Выведет с NRVO:
                     constructor
                     destructor
                     */
                }
            }
            std::cout << std::endl;
            {
                using namespace NO_NRVO;
                std::cout << "NO NRVO" << std::endl;
                
                /// 1 Пример
                {
                    std::cout << "1 Пример" << std::endl;
                    
                    A a;
                    [[maybe_unused]] auto result = function(a);
                    
                    /*
                     constructor
                     copy constructor
                     move constructor
                     destructor
                     destructor
                     destructor
                     */
                }
                std::cout << std::endl;
                /// 2 Пример
                {
                    std::cout << "2 Пример" << std::endl;
                    
                    A a;
                    [[maybe_unused]] auto result = function_ref(a);
                    
                    /*
                     constructor
                     copy constructor
                     destructor
                     destructor
                     */
                }
                std::cout << std::endl;
            }
        }
    }
}
