#include "Overload_Resolution.hpp"

#include <iostream>
#include <string>

namespace overload_resolution
{
    namespace access_to_base_class
    {
        struct Base
        {
            virtual ~Base() = default;
            void print(unsigned int number)
            {
                std::cout << "Base::print" << std::endl;
            }
            
            void print(float number)
            {
                std::cout << "Base::print" << std::endl;
            }
            
            void print(const std::string& str)
            {
                std::cout << "Base::print" << std::endl;
            }
            
            void function()
            {
                std::cout << "Base::function" << std::endl;
            }
        };

        struct Derived : Base
        {
            void print(unsigned int number)
            {
                std::cout << "Derived::print" << std::endl;
            }
        };
    }

    namespace using_access_to_base_class
    {
        struct Base
        {
            virtual ~Base() = default;
            void print(unsigned int number)
            {
                std::cout << "Base::print" << std::endl;
            }
            
            void print(float number)
            {
                std::cout << "Base::print" << std::endl;
            }
            
            void print(const std::string& str)
            {
                std::cout << "Base::print" << std::endl;
            }
            
            void function()
            {
                std::cout << "Base::function" << std::endl;
            }
        };

        struct Derived : Base
        {
            using Base::print; // разрешить доступ к перегруженным методам Base::print
            void print(unsigned int number)
            {
                std::cout << "Derived::print" << std::endl;
            }
        };
    }

    void Start()
    {
        // Разрешение перегрузки
        std::cout << "overload resolution" << std::endl;
        
        // Доступ переопределенной функции к базовому классу
        {
            std::cout << "Access of Overridden Function to the Base Class" << std::endl;
            using namespace access_to_base_class;
            
            std::unique_ptr<Base> base = std::make_unique<Base>();
            std::unique_ptr<Derived> derived = std::make_unique<Derived>();
            
            // Вызов перегруженного метода производного класса будет в приоритете
            {
                base->print(100u); // Base::print
                base->print(100.0f); // Base::print
                base->print("str"); // Base::function
                base->function(); // Base::function
                
                derived->print(100u); // Derived::print
                derived->print(100.0f); // Derived::print
                // derived->print("str"); Ошибка: Cannot initialize a parameter of type 'unsigned int' with an lvalue of type 'const char[4]'
                derived->function(); // Base::function
            }
            // Явный вызов метода принадлежности к базовому классу
            {
                derived->Base::print(10u); // Base::print
                derived->Base::print(100u); // Base::print
                derived->Base::print(100.0f); // Base::print
                derived->Base::print("str"); // Base::print
            }
        }
        std::cout << std::endl;
        // Область видимости для разрешения перегрузки можно расширить с помощью using
        {
            std::cout << "Access of Overridden Function to the Base Class by using" << std::endl;
            using namespace using_access_to_base_class;
            
            std::unique_ptr<Base> base = std::make_unique<Base>();
            std::unique_ptr<Derived> derived = std::make_unique<Derived>();
            
            base->print(100u); // Base::print
            base->print(100.0f); // Base::print
            base->print("str"); // Base::function
            
            // Тут требуется явное указание принадлежность в базовому классу
            derived->Base::print(10u); // Base::print
            derived->print(100u); // Derived::print
            derived->print(100.0f); // Base::print
            derived->print("str"); // Base::print
        }
        std::cout << std::endl;
    }
}
