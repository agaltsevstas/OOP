#include "Interview.hpp"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>

namespace Kaspersky
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

namespace Sber
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
}

namespace Yadro
{
    class A
    {
    public:
        A()
        {
            /*
             При выделении очень большой памяти может случиться std::bad_alloc -> при выбросе исключения объект не создасться и не будет вызыван деструктор -> утечка памяти -> лучше использовать RAII std::unique_ptr/std::shared_ptr для buffer1 для освобождения памяти в деструкторе.
             Лучше использовать std::make_unique, чем new.
             Плюсы:
             - невозможна утечка памяти.
             - не нужно писать new.
             - там не нужно дублировать тип unique_ptr/shared_ptr<int> number(new int(1)) -> auto number = make_unique/make_shared<int>(1).
             - make_shared - производит одно выделение памяти вместе: аллокация управляемого объекта + Control Block(shared_count (сильные ссылки), weak_count (слабые ссылки), allocator, deleter), следовательно они будут находиться в одном участке памяти и работать с ними быстрее, по сравнению с раздельным выделением памяти в shared_ptr.
             Минусы:
             - не могут использовать deleter.
             - перегруженные operator new и operator delete в классе будут проигнорированы в make_unique/make_shared.
             - make_shared не может вызывать private конструкторы внутри метода (например, синглтон).
             - для make_shared нужно ждать shared_count == weak_count == 0, чтобы удалить объект + Control Block.
             */
            
            buffer1 = new char[10]; // buffer1 = std::make_unique<char[]>(10);
            try
            {
                /*
                 Конструктор:
                 Исключение в конструкторе - единственное решение понять, что что-то пошло не так, иначе пришлось пробрасывать флаги, например: std::optional.
                 Проблема: если исключение вылетает в конструкторе, то объект не будет создан и получить доступ к объекту в состоянии ошибки невозможно. Это значит, что его деструктор не будет вызван. Исключение немедленно удаляет объект, также если выделялась динамическая память — возможная утечка памяти.
                 Решение:
                    1) Использовать умные указатели вместо сырых указателей, чтобы вызывался delete в деструкторе и не было утечки памяти.
                    2) Использовать в конструкторе обработку исключений try и в catch делать delete.
                    3) Способ обработки ошибок в конструкторе — это просто не пользоваться исправимыми техниками обработки ошибок. Следует использовать abort(), не допускающие продолжения работы программы.
                 Конструкторы не возвращают значения, поэтому это одна из причин, по которой исключения появились в С++.
                   4) Если нельзя обработать исправимую ошибку в конструкторе без исключений, тогда не использовать конструктор класса, а инициализировать в методе класса.
                 2. Деструктор:
                 - по-умолчанию исключения в деструкторе запрещены (noexcept) и всегда приводят к вызову std::terminate.
                 - если разрешить исключения (noexcept(false)) в деструкторе, то можно нарваться на double exception из-за другого исключения: в try/catch ловит первое исключение, то при попадании в catch идет раскрутка стека (stack unwiding/unrolling) и вызываются все деструкторы созданных ранее объектов, исключение из деструктора в catch блоке приводит к std:terminate.
                 - с помощью uncaught_exceptions(), который возвращает кол-во неперехваченных исключений в текущем потоке, можно избежать double exception в деструкторе, написав проверку if (!uncaught_exceptions()) throw.
                 Решение: в деструкторе должна быть гарантия отсутсвия исключений (exception safety).
                 */
                buffer2 = new char[1000000000000000]; // добавить try/catch, возможен std::bad_alloc
            }
            catch (std::bad_alloc& exception)
            {
                std::cout <<exception.what() << std::endl;
            }
            f(); // Ошибка: Pure virtual function called! Нужно определить метод или вынести из конструктора
        }
        
        /*
         Указателю на базовый класс присваивается динамический объект производного класса, то
         при разрушение объекта деструктор производного класса не вызовется, потому что удаление производится через указатель на базовый класс и для вызова деструктора компилятор использует раннее связывание (статический полиморфизм), поэтому может быть утечка памяти.
         Есть 2 случая для Base* base = new Derived:
         - Неполиморфный базовый класс: приводить производный класс к базовому можно, но нет смысла + при вызове delete может быть UB.
         - Полиморфный базовый класс: виртуальный деструктор использует позднее связывание (динамический полиморфизм), поэтому при разрушении объекта берется указатель, указывающий адрес деструктора производного класса из таблицы виртуальных функций, который после своей работы, вызывает деструктор базового класса.
         */
        ~A() // Дописать virtual
        {
            delete buffer1; // Для массива использовать buffer1[]
            delete buffer2; // Для массива использовать buffer2[]
        }
      
        virtual void f() = 0; // Можно сделать реализацию, чтобы в конструкторе не было ошибки: Pure virtual function called!

    private:
        char* buffer1; // Лучше заменить на std::unique_ptr<char[]> buffer1 (C++17), но не unique_ptr<int> buffer1 - будет первого элемента, для оставшихся - нет!!!
        char* buffer2; // Лучше заменить на std::unique_ptr<char[]> buffer2 (C++17), но не unique_ptr<int> buffer2 - будет первого элемента, для оставшихся - нет!!!
    };

    class B: public A
    {
    public:
        void f() // добавить override для virtual method
        {
            std::cout << "B::f";
        }
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

namespace working_file
{
    class IFile
    {
    public:
        virtual bool Connect() = 0;
        virtual void Disconnect() = 0;
        virtual std::vector<std::string> Read() = 0;
        virtual void Write(std::vector<std::string> iData) = 0;
        virtual ~IFile() {};
    };

    class File : public IFile
    {
    public:
        bool Connect() override
        {
            const std::string currrentPath = std::filesystem::current_path().string();
            const std::string fileName = "tmp.txt";
            const std::string directory = "/log/";
            std::filesystem::create_directory(currrentPath + directory); // Проверка на существование каталога. В случае отсутсвия, создается каталог
            
            std::string filePath = currrentPath + directory + fileName;
            
            try
            {
                _file.open(filePath, std::ios::in | std::ios::out);
                if (!_file.is_open())
                    throw std::ifstream::failure("Невозможно открыть директорию >> " + currrentPath + directory);
                
                _file.seekp(0, std::ios_base::beg);
                if (_file.peek() == EOF)
                {
                    throw "Пустой файл >> " + fileName;
                }
            }
            catch (const std::ifstream::failure& exception)
            {
                std::cout << exception.what() << std::endl;
                return false;
            }
            catch (const std::string& exception)
            {
                std::cout << exception << std::endl;
                return true;
            }
            catch (...)
            {
                std::cout << "Неизвестная ошибка" << std::endl;
                return false;
            }
            
            return true;
        }
        
        void Disconnect() override
        {
            _file.flush();
            _file.close();
        }
        
        std::vector<std::string> Read() override
        {
            std::vector<std::string> data;
            for (std::string line; getline(_file, line) ; )
                data.emplace_back(line);
            
            return data;
        }
        
        void Write(std::vector<std::string> iData) override
        {
            _file.clear();
            _file.seekp(0, std::ios_base::beg);
            for (const auto& data: iData)
                _file << data << std::endl;
        }
        
    private:
        std::fstream _file;
    };
}


namespace interview
{
    void start()
    {
        // Задача из компании Kaspersky
        {
            using namespace Kaspersky;
            std::cout << "exception" << std::endl;
            // Без try/catch
            {
#if 0
                [[maybe_unused]] E* e = new E();
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
        std::cout << std::endl;
        // Задача из компании Sber/Сбер
        {
            /*
             Конструктор: при создании объект производного класса, сначала вызывается конструктор базового класса, в этот момент еще НЕТ объекта производного класса, поэтому вызов виртуальной функции невозможен.
             Деструктор: в момент уничтожения объекта базового класса, объект производного класса уже уничтожен, поэтому вызов виртуальной функции невозможен.
             */
            using namespace Sber;
            std::cout << "Sber" << std::endl;
            
            // 1 Способ: no virtual method
            {
                using namespace no_virtual_method;
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
                using namespace virtual_method;
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
                using namespace virtual_destructor;
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
        // Задача из компании Yadro
        {
            using namespace Yadro;
            std::cout << "Yadro" << std::endl;
            
            /*
             Указателю на базовый класс присваивается динамический объект производного класса, то
             при разрушение объекта деструктор производного класса не вызовется, потому что удаление производится через указатель на базовый класс и для вызова деструктора компилятор использует раннее связывание (статический полиморфизм), поэтому может быть утечка памяти.
             Есть 2 случая для Base* base = new Derived:
             - Неполиморфный базовый класс: приводить производный класс к базовому можно, но нет смысла + при вызове delete может быть UB.
             - Полиморфный базовый класс: виртуальный деструктор использует позднее связывание (динамический полиморфизм), поэтому при разрушении объекта берется указатель, указывающий адрес деструктора производного класса из таблицы виртуальных функций, который после своей работы, вызывает деструктор базового класса.
             */
#if 0
            A* b = new B;
            delete b; // будет утечка памяти нужно добавить delete
#endif
        }
        // Задача из компании Order Capital
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
        // Задача на создание интерфейса к файлу (открытие, закрытие, чтение, запись)
        {
            using namespace working_file;
            
            File file;
            if (file.Connect())
            {
                auto data = file.Read();
                if (data.empty())
                {
                    std::vector<std::string> data = {"first", "second", "third"};
                    file.Write(data);
                }
                else
                {
                    file.Write(data);
                }
                
                file.Disconnect();
            }
        }
    }
}
