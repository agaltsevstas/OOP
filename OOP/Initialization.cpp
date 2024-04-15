#include "Initialization.hpp"

#include <iostream>

namespace initialization_order
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
        
        [[nodiscard]] std::shared_ptr<A> CreateA() { return std::make_shared<A>(); }
    };

    class C
    {
    public:
        C()
        {
            std::cout << "C()" << std::endl;
        }
        
        ~C()
        {
            std::cout << "~C()" << std::endl;
        }
    };

    class D
    {
    public:
        // Порядок инициализации членов класса D зависит от порядка их объявления, а не от порядка их в конструкторе!!!
        D() :
        _b(std::make_shared<B>()),
        _a(_b->CreateA()),
        _c(std::make_shared<C>())
        {
            /*
             Так вызовется оператор присваивания поверх вызова конструктора по умолчанию!!!
             _b = std::make_shared<B>();
             _a = _b->CreateA();
             _c = std::make_shared<C>();
             */
            std::cout << "D()" << std::endl;
        }
        
        ~D()
        {
            std::cout << "~D()" << std::endl;
        }
        
    private:                   // Порядок инициализации:
        std::shared_ptr<B> _b; // 1 - B()
        std::shared_ptr<A> _a; // 2 - A()
        std::shared_ptr<C> _c; // 3 - C()
    };

    class E
    {
    public:
        // Порядок инициализации членов класса D зависит от порядка их объявления, а не от порядка в конструкторе!!!
        E() :
        _b(std::make_shared<B>()), // может выдасть ошибку, т.к. класс B инициализируется после класса A
        _a(_b->CreateA()),
        _c(std::make_shared<C>())
        {
            /*
             Так вызовется оператор присваивания поверх вызова конструктора по умолчанию!!!
             _b = std::make_shared<B>();
             _a = _b->CreateA();
             _c = std::make_shared<C>();
             */
            std::cout << "E()" << std::endl;
        }
        
        ~E()
        {
            std::cout << "~E()" << std::endl;
        }
        
    private:                   // Порядок инициализации:
        std::shared_ptr<A> _a; // 1 - A() // Ошибка! т.к. сначала класс B, а потом A: _a(_b->CreateA())
        std::shared_ptr<B> _b; // 2 - B()
        std::shared_ptr<C> _c; // 3 - C()
    };

    void start()
    {
        // Порядок инициализации членов класса зависит от порядка их объявления, а не от порядка их в конструкторе!!!
        std::cout << "initialization order" << std::endl;
        
        // Верное объявление членов класса
        {
            D d;
            
            /* Вызов конструкторов и деструкторов классов работают на «стеке» LIFO («последним пришёл — первым ушёл»)
             B()
             A()
             C()
             D()
             ~D()
             ~C()
             ~A()
             ~B()
             */
        }
        std::cout << std::endl;
        // НЕверное объявление членов класса
        {
            E e;
            
            /* Вызов конструкторов и деструкторов классов работают на «стеке» LIFO («последним пришёл — первым ушёл»)
             A()
             B()
             C()
             E()
             ~E()
             ~C()
             ~B()
             ~A()
             */
        }
    }
};

namespace inheritance
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

    void start()
    {
        // Обычное наследование
        std::cout << "inheritance" << std::endl;
        C c;
        
        /* Вызов конструкторов и деструкторов классов работают на «стеке» LIFO («последним пришёл — первым ушёл»)
         A()
         B()
         C()
         ~C()
         ~B()
         ~A()
         */
        
        // Вызов методов базовых классов A::print() и B::print(), которые имеют одинаковые имена
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
}

