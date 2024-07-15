#include "Virtual.hpp"

#include <iostream>

/*
 Сайты: https://forum.shelek.ru/index.php/topic,9064.0.html
        https://habr.com/ru/articles/64280/
 */

namespace print
{
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
            
            ~Base() // need virtual destructor!!!
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

    namespace virtual_destructor
    {
        struct Base
        {
            Base()
            {
                print();
            }
            
            virtual void print()
            {
                std::cout << "Base::print" << std::endl;
            }
            
            virtual ~Base()
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
            
            void print() override
            {
                std::cout << "Derived::print" << std::endl;
            }
            
            ~Derived()
            {
                print(); // virtual не работает в деструкторе
            }
        };
    }
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
        
        ~Base() // need virtual destructor!!!
        {
            std::cout << "~Base" << std::endl;
        }
    };

    struct Derived : Base
    {
        Derived() : Base()
        {
            print(); // virtual не работает в конструкторе
        }
        
        void print() override
        {
            std::cout << "Derived::print" << std::endl;
        }
        
        ~Derived() // деструктор не вызовется
        {
            std::cout << "~Derived" << std::endl;
        }
    };
}

namespace no_virtual_destructor_inheritance
{
    struct A
    {
        int a = 0; // Обязательно для примера, чтобы была ошибка при delete!
        ~A()
        {
            std::cout << " ~A" << std::endl;
        }
    };

    struct B
    {
        int b = 0; // Обязательно для примера, чтобы была ошибка при delete!
        ~B()
        {
            std::cout << " ~B" << std::endl;
        }
    };

    struct C : public A, public B
    {
        int c = 0;
        ~C()
        {
            std::cout << " ~C" << std::endl;
        }
    };
}

namespace virtual_destructor
{
    struct Base
    {
        Base() = default;
        
        virtual void print()
        {
            std::cout << "Base::print" << std::endl;
        }
        
        virtual ~Base()
        {
            std::cout << "~Base" << std::endl;
        }
    };

    struct Derived : Base
    {
        Derived() = default;
        
        void print() override
        {
            std::cout << "Derived::print" << std::endl;
        }
        
        ~Derived()
        {
            std::cout << "~Derived" << std::endl;
        }
    };
}


namespace Virtual
{
    void Start()
    {
        // virtual
        std::cout << "virtual" << std::endl;
        /*
         Конструктор: при создании объект производного класса, сначала вызывается конструктор базового класса, в этот момент еще НЕТ объекта производного класса, поэтому вызов виртуальной функции невозможен.
         Деструктор: в момент уничтожения объекта базового класса, объект производного класса уже уничтожен, поэтому вызов виртуальной функции невозможен.
         */
        {
            // print
            {
                std::cout << "print" << std::endl;
                
                // 1 Способ: no virtual method
                {
                    using namespace print::no_virtual_method;
                    std::cout << "no virtual method" << std::endl;
                    
                    // reference
                    {
                        std::cout << "reference" << std::endl;
                        
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
                    // pointer
                    {
                        std::cout << "pointer" << std::endl;
                        
                        Base* base = new Derived;
                        base->print();
                        delete base;
                        
                        /* Вызов print на «стеке» LIFO («последним пришёл — первым ушёл»)
                         Base::print
                         Derived::print
                         Base::print
                         Base::print
                         */
                    }
                }
                std::cout << std::endl;
                // 2 Способ: virtual method
                {
                    using namespace print::virtual_method;
                    std::cout << "virtual method" << std::endl;
                    
                    // reference
                    {
                        std::cout << "reference" << std::endl;
                        
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
                    // pointer
                    {
                        std::cout << "pointer" << std::endl;
                        
                        Base* base = new Derived;
                        base->print();
                        delete base;
                        
                        /* Вызов print на «стеке» LIFO («последним пришёл — первым ушёл»)
                         Base::print
                         Derived::print
                         Derived::print
                         Base::print
                         */
                    }
                }
                std::cout << std::endl;
                // 3 Способ: virtual method + virtual destructor
                {
                    using namespace print::virtual_destructor;
                    std::cout << "virtual destructor" << std::endl;
                    
                    // reference
                    {
                        std::cout << "reference" << std::endl;
                        
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
                    // pointer
                    {
                        std::cout << "pointer" << std::endl;
                        
                        Base* base = new Derived;
                        base->print();
                        delete base;
                        
                        /* Вызов print на «стеке» LIFO («последним пришёл — первым ушёл»)
                         Base::print
                         Derived::print
                         Derived::print
                         Base::print
                         */
                    }
                }
                std::cout << std::endl;
            }
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
                
                // reference
                {
                    std::cout << "reference" << std::endl;
                    
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
                // pointer
                {
                    std::cout << "pointer" << std::endl;
                    
                    Base* base = new Derived;
                    base->print();
                    delete base;
                    
                    /* Вызов print на «стеке» LIFO («последним пришёл — первым ушёл»)
                     Base::print
                     Derived::print
                     Derived::print
                     Base::print
                     */
                }
            }
            /*
             2 Способ: Множественное наследование при отсутствии виртуального деструктора: при множественном наследовании, при удалении объекта производного класса через указатель на второй базовый класс, функции operator delete(void*) будет передан неправильный адрес, в результате чего программа упадет.
             */
            {
                using namespace no_virtual_destructor_inheritance;
                std::cout << "no virtual destructor inheritance" << std::endl;
                
                [[maybe_unused]] B* b = new C;
                /*
                 При приведении С* к B* к исходному указателю прибавляется 4 байта, чтобы он указывал на подобъект 2-го базового класса (B). При отсутствии виртального деструктора в строке адрес не будет скорректирован обратно на эти 4 байта, в результате чего получается удаление памяти по левому адресу, что приводит к abort/terminate.
                 */
#if 0
                delete pb;
#endif
            }
            std::cout << std::endl;
            // 3 Способ: virtual destructor
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
