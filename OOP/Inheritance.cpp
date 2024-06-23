#include "Inheritance.hpp"

#include <iostream>

/*
 Сайты: https://forum.shelek.ru/index.php/topic,9064.0.html
        https://habr.com/ru/articles/64280/
 */

/*
 Общая таблица спецификаторов доступа и типов наследования:
 В родительском классе | public в дочернем классе | private в дочернем классе | protected в дочернем классе
 public                | public                   | private                   | protected
 private               | Недоступен               | Недоступен                | Недоступен
 protected             | protected                | private                   | protected
 */

namespace public_inheritance
{
    class A
    {
    public:
        A()
        {
            std::cout << "A()" << std::endl;
        }
        
        ~A()
        {
            std::cout << "~A()" << std::endl;
        }
        
        void print()
        {
            std::cout << "class A" << std::endl;
        }
    };

    class B
    {
    public:
        B()
        {
            std::cout << "B()" << std::endl;
        }
        
        ~B()
        {
            std::cout << "~B()" << std::endl;
        }
        
        void print()
        {
            std::cout << "class B" << std::endl;
        }
    };

    class C : public A, public B
    {
    public:
        C() : A(), B() // Порядок вызовов конструкторов: 1 - A, 2 - B. Последовательность зависит от порядка определения: public A, public B
        {
            std::cout << "C()" << std::endl;
        }
        
        ~C()
        {
            std::cout << "~C()" << std::endl;
        }
    };
}

namespace protected_inheritance
{
    class Base
    {
    public:
        void print1()
        {
            std::cout << "public Base::print1" << std::endl;
        }
    protected:
        void print2()
        {
            std::cout << "protected Base::print2" << std::endl;
        }
        
    private:
        void print3()
        {
            std::cout << "private Base::print3" << std::endl;
        }
    };

    class Derived : protected Base
    {
    public:
        void function()
        {
            [[maybe_unused]] Base* base = new Base;
            [[maybe_unused]] Base* baseDerived = new Derived;
            [[maybe_unused]] Derived* derived = new Derived;
            [[maybe_unused]] auto object = static_cast<Base&>(*this);
            
            print1();
            print2();
            // print3(); // private
        }
    };
    
    class PublicDerived : public Derived
    {
    public:
        void function()
        {
            [[maybe_unused]] Base* base = new Base;
            [[maybe_unused]] Base* baseDerived = new Derived;
            [[maybe_unused]] Derived* derived = new Derived;
            [[maybe_unused]] auto object = static_cast<Base&>(*this);
            
            print1();
            print2();
            // print3(); // private
        }
    };

    class ProtectedDerived : protected Derived
    {
    public:
        void function()
        {
            [[maybe_unused]] Base* base = new Base;
            [[maybe_unused]] Base* baseDerived = new Derived;
            [[maybe_unused]] Derived* derived = new Derived;
            [[maybe_unused]] auto object = static_cast<Base&>(*this);
            
            print1();
            print2();
            // print3(); // private
        }
    };

    class PrivateDerived : private Derived
    {
    public:
        void function()
        {
            [[maybe_unused]] Base* base = new Base;
            [[maybe_unused]] Base* baseDerived = new Derived;
            [[maybe_unused]] Derived* derived = new Derived;
            [[maybe_unused]] auto object = static_cast<Base&>(*this);
            
            print1();
            print2();
            // print3(); // private
        }
    };
}

namespace private_inheritance
{
    class Base
    {
    public:
        void print1()
        {
            std::cout << "public Base::print1" << std::endl;
        }
    protected:
        void print2()
        {
            std::cout << "protected Base::print2" << std::endl;
        }
        
    private:
        void print3()
        {
            std::cout << "private Base::print3" << std::endl;
        }
    };

    class Derived : private Base
    {
    public:
        void function()
        {
            [[maybe_unused]] Base* base = new Base;
            [[maybe_unused]] Base* baseDerived = new Derived;
            [[maybe_unused]] Derived* derived = new Derived;
            [[maybe_unused]] auto object = static_cast<Base&>(*this);
            
            print1();
            print2();
            // print3(); // private
        }
    };

    class DerivedDerived : public Derived
    {
    public:
        void function()
        {
            // [[maybe_unused]] Base* base = new Base; // private
            // [[maybe_unused]] Base* baseDerived = new Derived; // private
            // [[maybe_unused]] Derived* derived = new Derived; //
            // [[maybe_unused]] auto object = static_cast<Base&>(*this); // private
            
            // print1(); private
            // print2(); private
            // print3(); // private
        }
    };
}

namespace diamond_inheritance
{
    class A
    {
    public:
        A()
        {
            std::cout << "A()" << std::endl;
        }
        
        ~A()
        {
            std::cout << "~A()" << std::endl;
        }
        
        void print()
        {
            std::cout << "class A" << std::endl;
        }
    };

    class B : public virtual A // Вызовется один консруктор
    //class B : public A // Вызовется еще один конструктор
    {
    public:
        B()
        {
            std::cout << "B()" << std::endl;
        }
        
        ~B()
        {
            std::cout << "~B()" << std::endl;
        }
        
        void print()
        {
            std::cout << "class A" << std::endl;
        }
    };

    class C : public virtual A // Вызовется один консруктор
    //class C : public A // Вызовется еще один конструктор
    {
    public:
        C() : A()
        {
            std::cout << "C()" << std::endl;
        }
        
        ~C()
        {
            std::cout << "~C()" << std::endl;
        }
        
        void print()
        {
            std::cout << "class A" << std::endl;
        }
    };

    class D : public B, public C
    {
    public:
        D() : B(), C() // Порядок вызовов конструкторов: 1 - B, 2 - C. Последовательность зависит от порядка определения: public B, public C
        {
            std::cout << "D()" << std::endl;
        }
        
        ~D()
        {
            std::cout << "~D()" << std::endl;
        }
        
        void print()
        {
            std::cout << "class A" << std::endl;
        }
    };
}

namespace no_virtual_destructor_inheritance
{
    struct A
    {
        ~A()
        {
            std::cout << " ~A" << std::endl;
        }
        
        int a = 0; // Обязательно для примера, чтобы была ошибка при delete!
    };

    struct B
    {
        ~B()
        {
            std::cout << " ~B" << std::endl;
        }
        
        int b = 0; // Обязательно для примера, чтобы была ошибка при delete!
    };

    struct C : public A, public B
    {
        ~C()
        {
            std::cout << " ~C" << std::endl;
        }
        
        int c = 0;
    };
}

namespace inheritance
{
    void start()
    {
        // public наследование
        {
            using namespace public_inheritance;
            std::cout << "public inheritance" << std::endl;
            C c;
            
            /* Вызов конструкторов и деструкторов классов работают на «стеке» LIFO («последним пришёл — первым ушёл»)
             A()
             B()
             C()
             ~C()
             ~B()
             ~A()
             */
            
            // Явный вызов методов базовых классов A::print() и B::print(), которые имеют одинаковые имена
            {
                // c.print(); // Ошибка: есть неопределеннсоть вызова либо A::print(), либо B::print()
                c.A::print(); // Вызов A::print()
                c.B::print(); // Вызов либо B::print()
            }
            
            // Альтернативный вызов методов базовых классов A::print() и B::print() через static_cast, которые имеют одинаковые имена
            {
                static_cast<A&>(c).print(); // Вызов A::print()
                static_cast<B&>(c).print(); // Вызов либо B::print()
            }
        }
        // protected наследование
        {
            using namespace protected_inheritance;
            std::cout << "protected inheritance" << std::endl;
            
            // Base* basePtr = new Derived; // Cannot cast 'Derived' to its protected base class 'Base'
            [[maybe_unused]] Derived* DerivedPtr = new Derived;
            
            Base base;
            base.print1();
            // base.print2(); // protected
            // base.print3(); // private
            
            Derived derived;
            derived.function(); // public
            
            // static_cast<Base&>(derived); // Cannot cast 'Derived' to its protected base class 'Base'
            
            // derived.print1(); // protected
            // derived.print2(); // protected
            // derived.print3(); // protected
            
            // Явный вызов методов базового классу Base
            {
                // derived.Base::print1(); // 'Base' is a protected member
                // derived.Base::print2(); // 'Base' is a protected member
                // derived.Base::print3(); // 'Base' is a protected member
            }
            
            // Альтернативный вызов методов базового класса Base
            {
                // static_cast<Base&>(derived).print1(); // Cannot cast 'Derived' to its protected base class 'Base'
                // static_cast<Base&>(derived).print2(); // Cannot cast 'Derived' to its protected base class 'Base'
                // static_cast<Base&>(derived).print3(); // Cannot cast 'Derived' to its protected base class 'Base'
            }
        }
        // private наследование
        {
            using namespace private_inheritance;
            std::cout << "private inheritance" << std::endl;
            
            // Base* basePtr = new Derived; // Cannot cast 'Derived' to its protected base class 'Base'
            [[maybe_unused]] Derived* DerivedPtr = new Derived;
            
            Base base;
            base.print1();
            // base.print2(); // protected
            // base.print3(); // private
            
            Derived derived;
            derived.function(); // public
            
            // static_cast<Base&>(derived); // Cannot cast 'Derived' to its private base class 'Base'
            
            // derived.print1(); // protected
            // derived.print2(); // protected
            // derived.print3(); // protected
            
            // Явный вызов методов базового классу Base
            {
                // derived.Base::print1(); // 'Base' is a private member
                // derived.Base::print2(); // 'Base' is a private member
                // derived.Base::print3(); // 'Base' is a private member
            }
            
            // Альтернативный вызов методов базового класса Base
            {
                // static_cast<Base&>(derived).print1(); // Cannot cast 'Derived' to its private base class 'Base'
                // static_cast<Base&>(derived).print2(); // Cannot cast 'Derived' to its private base class 'Base'
                // static_cast<Base&>(derived).print3(); // Cannot cast 'Derived' to its private base class 'Base'
            }
        }
        // Ромбовидное наследование
        {
            std::cout << "diamond inheritance" << std::endl;
            
            diamond_inheritance::D d;
            
            /* Вызов конструкторов и деструкторов классов работают на «стеке» LIFO («последним пришёл — первым ушёл»)
             A()
             B()
             C()
             D()
             ~D()
             ~C()
             ~B()
             ~A()
             */
        }
        /*
         Множественное наследование при отсутствии виртуального деструктора: при множественном наследовании, при удалении объекта производного класса через указатель на второй базовый класс, функции operator delete(void*) будет передан неправильный адрес, в результате чего программа упадет.
         */
        {
            using namespace no_virtual_destructor_inheritance;
            std::cout << "no virtual destructor inheritance" << std::endl;
            
            [[maybe_unused]] B* b = new C;
            /*
             При приведении С* к B* к исходному указателю прибавляется 4 байта, чтобы он указывал на подобъект 2-го базового класса (B). При отсутствии виртального деструктора в строке адрес не будет скорректирован обратно на эти 4 байта, в результате чего получается удаление памяти по левому адресу, что приводит к abort/terminate.
             */
#if 0
            delete b;
#endif
        }
    }
}
