#include "Diamond_Inheritance.hpp"

#include <iostream>

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

    void start()
    {
        std::cout << "diamond inheritance" << std::endl;
        // Ромбовидное наследование
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
}
