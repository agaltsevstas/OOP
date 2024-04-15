#include "Virtual.hpp"

#include <iostream>

namespace no_virtual_method
{
    struct Base
    {
        Base()
        {
            print();
        }
        
        void print()
        {
            std::cout << "Base::print" << std::endl;
        }
        
        ~Base()
        {
            print();
        }
    };

    struct Derived : Base
    {
        Derived() : Base()
        {
            print(); // virtual не работает в конструкторе
        }
        
        virtual void print()
        {
            std::cout << "Derived::print" << std::endl;
        }
        
        ~Derived()
        {
            print(); // virtual не работает в деструкторе
        }
    };
}

namespace virtual_method
{
    struct Base
    {
        Base()
        {
            print(); // virtual не работает в конструкторе
        }
        
        virtual void print()
        {
            std::cout << "Base::print" << std::endl;
        }
        
        ~Base() // virtual destructor!!!
        {
            print(); // virtual не работает в деструкторе
        }
    };

    struct Derived : Base
    {
        Derived() : Base()
        {
            print(); // virtual не работает в конструкторе
        }
        
        virtual void print()
        {
            std::cout << "Derived::print" << std::endl;
        }
        
        ~Derived()
        {
            print(); // virtual не работает в деструкторе
        }
    };
}

namespace non_virtual_interface
{
    struct Base
{
        Base()
        {
            print(); // virtual не работает в конструкторе
        }
        
        void print()
        {
            print_impl();
        }
        
        virtual void print_impl() = 0;
        virtual ~Base() = default;
    };

    struct Derived : Base
    {
        void print_impl() override
        {
            std::cout << "Derived::print" << std::endl;
        }
    };
}

namespace virtual_method_no_override
{
    struct Base
    {
        virtual ~Base() = default;
        virtual void print(size_t number)
        {
            std::cout << "Base::print" << std::endl;
        }
    };

    struct Derived : Base
    {
        virtual void print(float number)
        {
            std::cout << "Derived::print" << std::endl;
        }
    };
}

namespace no_virtual_destructor
{
    struct Base
    {
        Base()
        {
            print(); // virtual не работает в конструкторе
        }
        
        virtual void print()
        {
            std::cout << "Base::print" << std::endl;
        }
        
        ~Base() // virtual destructor!!!
        {
            print(); // virtual не работает в деструкторе
        }
    };

    struct Derived : Base
    {
        Derived() : Base()
        {
            print(); // virtual не работает в конструкторе
        }
        
        virtual void print()
        {
            std::cout << "Derived::print" << std::endl;
        }
        
        ~Derived() // деструктор не вызовется
        {
            print(); // virtual не работает в деструкторе
        }
    };
}

namespace virtual_destructor
{
    struct Base
    {
        Base()
        {
            print(); // virtual не работает в конструкторе
        }
        
        virtual void print()
        {
            std::cout << "Base::print" << std::endl;
        }
        
        virtual ~Base()
        {
            print(); // virtual не работает в деструкторе
        }
    };

    struct Derived : Base
    {
        Derived() : Base()
        {
            print(); // virtual не работает в конструкторе
        }
        
        void print()
        {
            std::cout << "Derived::print" << std::endl;
        }
        
        ~Derived()
        {
            print(); // virtual не работает в деструкторе
        }
    };
}

namespace Virtual
{
    void start()
    {
        // virtual
        std::cout << "virtual" << std::endl;
        /*
         Конструктор: при создании объект производного класса, сначала вызывается конструктор базового класса, в этот момент еще НЕТ объекта производного класса, поэтому вызов виртуальной функции невозможен.
         Деструктор: в момент уничтожения объекта базового класса, объект производного класса уже уничтожен, поэтому вызов виртуальной функции невозможен.
         */
        {
            // 1 Способ: no virtual method
            {
                using namespace no_virtual_method;
                std::cout << "no virtual method" << std::endl;
                
                Derived derived;
                Base& base = derived;
                base.print();
                
                /* Вызов print на «стеке» LIFO («последним пришёл — первым ушёл»)
                 Base::print
                 Derived::print
                 Base::print
                 Derived::print
                 Base::print
                 */
            }
            std::cout << std::endl;
            // 2 Способ: virtual method
            {
                using namespace virtual_method;
                std::cout << "virtual method" << std::endl;
                
                Derived derived;
                Base& base = derived;
                base.print();
                
                /* Вызов print на «стеке» LIFO («последним пришёл — первым ушёл»)
                 Base::print
                 Derived::print
                 Derived::print
                 Derived::print
                 Base::print
                 */
            }
            std::cout << std::endl;
            /*
             3 Способ: NVI (non virtual interface) - шаблона невиртуального интерфейса использует public невиртуальные методы в качестве обертки над вызовами private/protected виртуальных методов. NVI - частный случай паттерна — «Шаблонный метод». Преимущество идиомы NVI - выполнение предварительных действий перед вызовов виртуального метода и выполнение завершающих действий после вызова виртуального метода (например, захват mutex).
             Плюсы:
             - базовый класс (интерфейс) имеет больше контроля над своим поведением, чем производные классы (например, вывод в файл или экран не нужно дублировать в каждой реализации производного класса: достаточно вынести вызов виртуального метода в невиртуальный метод + дополнив его).
             Минусы:
             - лишние методы.
             */
            {
                using namespace non_virtual_interface;
                std::cout << "NVI (non virtual interface)" << std::endl;
                
                // Derived derived; // Ошибка: Pure virtual function called!
            }
            std::cout << std::endl;
            // 3 Способ: virtual method no override
            {
                using namespace virtual_method_no_override;
                std::cout << "virtual method no override" << std::endl;
                
                std::unique_ptr<Base> base(new Derived);
                base->print(100u); // Base::print
                base->print(100.0f); // Base::print
                
                auto derived = std::make_unique<Derived>();
                derived->print(100u); // Derived::print
                derived->print(100.0f); // Derived::print
                derived->Base::print(100u); // Base::print
                derived->Base::print(100.0f); // Base::print
            }
            std::cout << std::endl;
        }
        /*
         Виртуальный деструктор нужно делать ТОЛЬКО для полиморфного базового класса (имеет хотя 1 virtual method), иначе - делать не нужно.
         Если указателю на производного класс присваивается динамический объект производного класса, то при разрушение объекта деструктор производного класса вызовется.
         Если указателю на базовый класс присваивается динамический объект производного класса, то
         при разрушение объекта деструктор производного класса не вызовется. Для этого используется виртуальный деструктор в базовом классе.
         */
        {
            // 1 Способ: no virtual destructor
            {
                using namespace no_virtual_destructor;
                std::cout << "no virtual destructor" << std::endl;
                
                Base* base = new Derived;
                base->print();
                delete base; // деструктор производного класса не вызовется
                
                /* Вызов print на «стеке» LIFO («последним пришёл — первым ушёл»)
                 Base::print
                 Derived::print
                 Derived::print
                 */
            }
            std::cout << std::endl;
            // 2 Способ: virtual destructor
            {
                using namespace virtual_destructor;
                std::cout << "virtual destructor" << std::endl;
                
                Base* base = new Derived;
                base->print();
                delete base;
                
                /* Вызов print на «стеке» LIFO («последним пришёл — первым ушёл»)
                 Base::print
                 Derived::print
                 Derived::print
                 Derived::print
                 Base::print
                 */
            }
            std::cout << std::endl;
        }
    }
}
