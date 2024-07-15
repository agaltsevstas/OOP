#include "ADL.hpp"

#include <iostream>
#include <string>

namespace ADL
{
    class A {};

    void function(int number)
    {
        
    }

    void function(const A& a, int number)
    {
        
    }

    void Start()
    {
        /* ADL (argument-dependent lookup) - поиск по аргументу, можно не указывать пространство имен к функции, если один из аргументов принадлежит к тому же пространству имен и он уже указан. Компилятор ищет функцию в пространствах имен в типах аргументов.
         */
        {
            ADL::A a;
            /// Без ADL:
            {
                ADL::function(0);
                ADL::function(a, 0);
            }
            /// С ADL:
            {
                function(a, 0);
            }
        }
        /// ADL в stl
        {
            std::string str = "hello world";
            
            /// Без ADL:
            {
                std::begin(str);
                operator<<(std::cout, str);
                std::cout.operator<<(str.c_str());
            }
            /// С ADL:
            {
                begin(str);
                std::cout << str;
            }
        }
    }
}
