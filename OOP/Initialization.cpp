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
}

namespace exception
{
    struct A
    {
        A() { std::cout << "A()" << std::endl; }
        ~A() { std::cout << "~A()" << std::endl; }
    };

    struct B
    {
        B() { std::cout << "B()" << std::endl; }
        ~B() { std::cout << "~B()" << std::endl; }
    };

    struct C
    {
        C() { std::cout << "C()" << std::endl; }
        ~C() { std::cout << "~C()" << std::endl; }
    };

    struct D
    {
        D() { std::cout << "D()" << std::endl; }
        ~D() { std::cout << "~D()" << std::endl; }
        
        A a;
        B b;
    };

    struct E : private D
    {
        /*
         Порядок инициализации не соответсвует порядку объявления в классе, может быть UB, должно быть:
         E():
         nullptr,
         b(),
         c()
         */
        E():
        c(),
        b(),
        a() // nullptr, порядок инициализации не соответсвует порядку объявления в классе, но т.к. это указатель == nullptr -> UB не будет
        {
            std::cout << "E()" << std::endl;
            a.reset(new A); // при исключении утечки не будет, потому что RAII (захват в конструкторе, освобождение в деструкторе)
            throw std::runtime_error("err");
        }

        ~E()
        {
            std::cout << "~E()" << std::endl; // Не вызовется, т.к. объект не сконструируется
        }

        std::unique_ptr<A> a;
        B b;
        C c;
    };
}

namespace Order_Capital
{
    struct A
    {
        ~A()
        {
            std::cout << "~A()" << std::endl;
        }
    };

    struct B : A
    {
        ~B()
        {
            std::cout << "~B()" << std::endl;
        }
    };
}

namespace initialization
{
    void start()
    {
        // Порядок инициализации членов класса зависит от порядка их объявления, а не от порядка их в конструкторе!!!
        {
            using namespace initialization_order;
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
            std::cout << std::endl;
        }
        // Порядок вывода при бросании исключения и раскрутки стека
        {
            using namespace exception;
            std::cout << "exception" << std::endl;
            // Без try/catch
            {
#if 0
                E* e = new E();
                delete e;
#endif
                
                /* Вызов конструкторов и деструкторов классов работают на «стеке» LIFO («последним пришёл — первым ушёл»)
                 A()
                 B()
                 D()
                 B()
                 C()
                 E()
                 A()
                 */
            }
            /*
             try/catch - помогает отлавливать исключения, чтобы произошла раскрутка стека (stack unwiding/unrolling) и в момент попадания в catch вызывался деструктор всех объектов, которые вышли из стека. Деструктор помогает перед выходом программы сохранить какие-то пользовательские изменения. Ошибочно считать, что деструктор объекта вызовется при выходе из стека.
             Непойманные исключения приводят к завершению программы через std::terminate.
             */
            {
                try
                {
                    [[maybe_unused]] E* e = new E();
                }
                catch (std::runtime_error ex) //
                {
                    
                }
                
                /* Вызов конструкторов и деструкторов классов работают на «стеке» LIFO («последним пришёл — первым ушёл»)
                 A()
                 B()
                 D()
                 B()
                 C()
                 E()
                 A()
                 ~C()
                 ~B()
                 ~A()
                 ~D()
                 ~B()
                 ~A()
                 */
            }
        }
        // Разница вызовов деструкторов при std::shared_ptr/unique_ptr
        {
            using namespace Order_Capital;
            std::cout << "Order Capital" << std::endl;
            
            /* Деструктор struct A - невиртуальный, поэтому должно вызваться только деструктор: ~A() - это неверно. std::shared_ptr хранит внутри себя deleter, который знает тип объекта, переданного в конструктор, и поэтому будут вызываться все деструкторы:
             ~B()
             ~A()
             */
            
            std::shared_ptr<A> base_ptr1 = std::make_shared<B>();
            /* Вызов деструкторов классов работают на «стеке» LIFO («последним пришёл — первым ушёл»)
             ~B()
             ~A()
             */
            
            /*
             Если заменить std::shared_ptr на std::unique_ptr, то в std::unique_ptr deleter является частью типа, поэтому будет вызываться только деструктор:
             ~A()
             */
            
            std::unique_ptr<A> base_ptr2 = std::make_unique<B>();
            /* Вызов деструкторов классов работают на «стеке» LIFO («последним пришёл — первым ушёл»)
             ~A()
             */
        }
        std::cout << std::endl;
    }
};
