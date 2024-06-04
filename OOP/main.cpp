#include "ADL.hpp"
#include "Aligment.hpp"
#include "Declaration_Definition.hpp"
#include "Initialization.hpp"
#include "Inheritance.hpp"
#include "Interview.hpp"
#include "Overload_Resolution.hpp"
#include "POD.hpp"
#include "Virtual.hpp"

#include <iostream>
#include <vector>

/*
 Инициализация
 Сайты: https://learn.microsoft.com/ru-ru/cpp/cpp/initializers?view=msvc-170
        https://habr.com/ru/companies/jugru/articles/469465/
        https://ru.stackoverflow.com/questions/616184/%D0%9A%D0%B0%D0%BA-%D1%80%D0%B0%D0%B1%D0%BE%D1%82%D0%B0%D0%B5%D1%82-alignas
 
 
Declaration and definition
Лекция: https://www.youtube.com/watch?v=c7CasTJKw7o&list=PLlb7e2G7aSpTFea2FYxp7mFfbZW-xavhL&index=11&ab_channel=ComputerScienceCenter
Сайты: https://habr.com/ru/companies/otus/articles/432834/
    https://habr.com/ru/companies/jugru/articles/506104/
 
POD
Видео: https://www.youtube.com/watch?v=KqqrJYEUeTw&ab_channel=cppProsto
Сайты: https://habr.com/ru/articles/470265/
       https://learn.microsoft.com/ru-ru/cpp/cpp/trivial-standard-layout-and-pod-types?view=msvc-170
       https://habr.com/ru/articles/532972/
       https://ru.wikipedia.org/wiki/%D0%9F%D1%80%D0%BE%D1%81%D1%82%D0%B0%D1%8F_%D1%81%D1%82%D1%80%D1%83%D0%BA%D1%82%D1%83%D1%80%D0%B0_%D0%B4%D0%B0%D0%BD%D0%BD%D1%8B%D1%85
  
 */


namespace declaration_definition // Объявление
{
    void print(); // без объявления нельзя будет вызвать функцию

    inline void print_number() // в каждой единице трансляции (.cpp) должен быть свой inline
    {
        std::cout << "number" << std::endl;
    }
}


int main()
{
    /*
     Числа с плавающей точкой: сравнение должно быть в определенном диапазоне, а само число должно быть взято по модулю.
     */
    {
        double number = 2.0;
        if (number <= 2.1 && number >= 1.9)
        {
            std::cout << "number = 2.0" << std::endl;
        }
    }
    /*
     Массивы
     */
    {
        /// Можно поменять индексы и имя массива местами.
        {
            int mas[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
            [[maybe_unused]] auto compare = mas[0] == *(mas + 0) == *(0 + mas) == 0[mas]; // одинаковый доступ к 0 элементу
            [[maybe_unused]] int number = 3[mas - 1] - mas[3] + (mas - 1)[5]; // mas[2] - mas[3] + (mas[5] - 1) = 3 - 4 + (6 - 1) = 4
        }
        /// Арифметика указателей
        {
            char mas[] = {1,1,1,1, 1,1,1,1, 1,1,1,1};
            *((int*)mas + 2) = 0; // int = 4 байта => 4 * 2 = 8, зануляем последние 4 байта, начиная с 8 элемента
            for (int i = 0, I = sizeof(mas)/sizeof(mas[0]); i < I; ++i)
                std::cout << (int)mas[i] << ",";
            std::cout << std::endl;
        }
    }
    /*
     Переполнение безнакового числа: Бесконечный цикл
     */
    {
        // 1 Пример
        {
            bool overflow = false;
            for (uint8_t i = 0; i < 256; ++i)
            {
                if (i == 255)
                    overflow = true;
                else if (i == 1 && overflow)
                    break;
                std::cout << "Число: " << (int)i << std::endl;
            }
        }
        // 2 Пример
        {
            unsigned char count = 128; // 1000 0000 = 2^7 = 128
            
            // Цикл не начинается: приведение типа к unsigned char
            {
                [[maybe_unused]] auto offset = (unsigned char) (count << 1); // // 0000 0000 = 0
                for (unsigned char i = 0; i < (unsigned char) (count << 1); ++i) // 0 < 0
                    std::cout << "Число: " << (int)i << std::endl;
            }
            // Бесконечный цикл: : приведение типа к int
            {
                bool overflow = false;
                [[maybe_unused]] auto offset = (count << 1); // // 1 0000 0000 = 2^8 = 256
                for (unsigned char i = 0; i < (count << 1); ++i) // 0 < 0
                {
                    if (i == 255)
                        overflow = true;
                    else if (i == 1 && overflow)
                        break;
                    std::cout << "Число: " << (int)i << std::endl;
                }
            }
        }
    }
    /*
     Указатели. Отличие указателя от константного указателя: если const находится слева от * - это указатель на константу, если const находится справа от * - это константный указатель. Если const находится слева и справа от * - это константный указатель на константную переменную.
     */
    {
        int value = 1;
        
        /// Указатель на константу: можно менять только указатель. Оба варианта идентичны:
        {
            const int* const_int_ptr = &value;
            int const* int_const_ptr = &value;
            
            // *const_int_ptr = 10; // Ошибка: Read-only variable is not assignable
            const_int_ptr = nullptr;
            
            // *int_const_ptr = 10; // Ошибка: Read-only variable is not assignable
            int_const_ptr = nullptr;
        }
        /// Константные указатели. Оба варианта НЕидентичны:
        {
            /// Константный указатель на переменную:
            {
                int* const int_ptr_const = &value;
                
                *int_ptr_const = 10;
                // int_ptr_const = nullptr; // Ошибка: Cannot assign to variable with const-qualified type 'int *const'
            }
            /// Константный указатель на константную переменную:
            {
                [[maybe_unused]] const int* const const_int_ptr_const = &value;
                
                // *int_ptr_const = 10; // Ошибка: Read-only variable is not assignable
                // int_ptr_const = nullptr; // Ошибка: Cannot assign to variable with const-qualified type 'int *const'
            }
        }
    }
    /* ADL (argument-dependent lookup) - поиск по аргументу, можно не указывать пространство имен к функции, если один из аргументов принадлежит к тому же пространству имен и он уже указан. Компилятор ищет функцию в пространствах имен в типах аргументов.
     */
    {
        using namespace ADL;
        std::cout << "ADL" << std::endl;
        
        start();
    }
    std::cout << std::endl;
    /*
     Объявлений может быть много в разных единицах трансляциях (.cpp), но определение должно быть только одно, иначе будет переопределение (redefinition). Лучше объявление выносить в заголовочный файл (.h).
     */
    {
        std::cout << "internal linkage" << std::endl;
        std::cout << "static number1: " << &internal_linkage::number1 << std::endl;
        std::cout << "const number2: " << &internal_linkage::number2 << std::endl;
        std::cout << "constexpr number3: " << &internal_linkage::number3 << std::endl;
        internal_linkage::print();
        std::cout << std::endl;
        
        std::cout << "external linkage" << std::endl;
        std::cout << "extern number1: " << &external_linkage::number1 << std::endl;
        std::cout << "inline number2: " << &external_linkage::number2 << std::endl;
        std::cout << "static constexpr number: " << &external_linkage::Number::number << std::endl;
        external_linkage::print();
        std::cout << std::endl;
        
        declaration_definition::print();
        declaration_definition::print_number();
    }
    // virtual
    {
        Virtual::start();
    }
    // Разрешение перегрузки
    {
        overload_resolution::start();
    }
    /*
     Инициализация - язык от C унаследованы 5 типов инициализации: 1) инициализация по умолчанию, 2) нулевая инициализация, 3) копирующая инициализация, 4) агрегатная и 5) статическая инициализация.
     язык С++: прямая инициализация (С++98), инициализация значением (С++03), универсальная инициализация (инициализация списком) (C++11), назначенная инициализация (С++20), прямая инициализация агрегатных типов (ЕЩЕ НЕ СДЕЛАЛИ в C++23).
     */
    {
        /*
         1) Инициализация по умолчанию — это инициализация с помощью конструктора по умолчанию. Если класс, структура или объединение не имеет конструктор по умолчанию, компилятор выдает ошибку.
         */
        {
            [[maybe_unused]] int i;
        }
        /*
         2) Нулевая инициализация - это инициализируются с помощью (ПУСТЫХ) фигурных скобок.
         */
        {
            [[maybe_unused]] int i{};
        }
        /*
         3) Инициализация копированием — это инициализация одного объекта с использованием другого объекта. Инициализация копированием не может вызывать явные (explicit) конструкторы.
         */
        {
            struct Widget
            {
                explicit Widget(int) {}
            };

            // Widget w = 1; // ошибка: explicit
            [[maybe_unused]] int i = 0;
        }
        /*
         4) Агрегатная инициализация - это инициализация с использованием (НЕпустых) фигурных скобок без использования конструктора при отсутствии НЕзакрытых членов класса/структуры в последовательном порядке, где можно при желании пропустить инициализацию последних членов класса/структуры для них используется нулевая инициализация. Агрегатная инициализация использует копирующую инициализацию для каждого элемента. В отличие от копируемой инициализации она НЕ может вызывать явные (explicit) конструкторы.
         */
        {
            [[maybe_unused]] int i[4] = {0, 1, 2, 3};
            struct Widget
            {
                Widget(int i, int j) : i(i), j(j) {}
                int i;
                int j;
            };
            
            struct Thingy
            {
                Widget w;
                int k;
            };
            
            [[maybe_unused]] Thingy t1 = { Widget(1, 2), 3 };
            [[maybe_unused]] Thingy t2 = { Widget(1, 2) };
            [[maybe_unused]] Thingy t3 { t1 };
        }
        /*
         5) Статическая инициализация - статические переменные ВСЕГДА инициализируются!
         */
        {
            [[maybe_unused]] static int i1 = 0; // инициализация 0
            [[maybe_unused]] static int i2;     // инициализация 0
        }
        /*
         6) Прямая инициализация (С++98) — это инициализация с использованием (НЕпустых) круглых/фигурных скобок и выполняется ТОЛЬКО для конструкторов при помощи разрешения перегрузки. В отличие от копируемой инициализации она может вызывать явные (explicit) конструкторы.
         */
        {
            struct Widget
            {
                explicit Widget(int ii): i(ii) {}
                int i;
            };

            // Widget w1 = 1; // ошибка: explicit
            Widget w2(1);  // можно
            Widget w3{1};  // можно
        }
        /*
         7) Инициализация со значением (С++03) — это инициализация с использованием (пустых) круглых при наличии ОПРЕДЕЛЕННЫМ ПОЛЬЗОВАТЕЛЕМ конструктором по умолчанию, инициализация значением вызывает этот конструктор, в противном случае возвращается нуль.
         */
        {
            [[maybe_unused]] int i = int(); // C++98: UB, C++03: 0
            struct Widget1
            {
                int i; // i == 0
            };
                
            struct Widget2
            {
                Widget2() {}
                int i; // i == UB
            };
                
            struct Widget3
            {
                Widget3() = default; // конструктор определен пользователем, но не предоставлен пользователоем (user-defined, но не user-provided)
                int i; // i == 0
            };
            
            auto widget1 = Widget1();
            auto widget2 = Widget2();
            auto widget3 = Widget3();
            widget1.i; // i == 0
            widget2.i; // i == UB
            widget3.i; // i == UB
        }
        /*
         8) Универсальная инициализация (инициализация списком) (C++11) - это инициализация с использованием braced-init-list - std::initializer_list, бывает двух типов: прямая и копирования.
         */
        {
            std::vector<int> vec1 {0, 1, 2, 3, 4}; // direct-list-initialization
            std::vector<int> vec2 = {0, 1, 2, 3, 4}; // copy-list-initialization
            
            // Могут быть проблемы с ПРЯМОЙ инициализацией
            {
                std::vector<int> v3(3, 0); // вектор содержит 0, 0, 0
                std::vector<int> v4{3, 0}; // вектор содержит 3, 0
            }
            // Могут быть проблемы с конструктором по умолчанию
            {
                struct Widget1
                {
                    Widget1(){} // конструктор по умолчанию
                    Widget1(const std::initializer_list<int>& list) {}
                    
                    int i;
                };
                
                struct Widget2
                {
                    Widget2() = default;
                    int i;
                };
                
                Widget1 widget1{}; // вызовется конструктор по умолчанию
                widget1.i; // i == UB
                Widget2 widget2{}; // вызовется конструктор по умолчанию
                widget2.i; // i == 0
            }
        }
        /*
         9) Назначенная инициализация (С++20) - это агрегатная инициализация с другим синтаксисом - можно не соблюдать порядок элементов.
         */
        {
            struct Widget
            {
               int a;
               int b;
               int c;
            };
            
            struct Thingy
            {
               Widget w;
               int d;
            };
            
            [[maybe_unused]] Widget widget {.a = 1, .c = 3};
            [[maybe_unused]] Thingy thingy = {.w{.a = 1, .b = 2, .c = 3}, .d = 4};
        }
        /*
         10) Прямая инициализация агрегатных типов (ЕЩЕ НЕ СДЕЛАЛИ в C++23) - это прямая инициализация + агрегатная инициализация, с использованием (НЕпустых) КРУГЛЫХ скобок без использования конструктора при отсутствии НЕзакрытых членов класса/структуры в последовательном порядке.
         */
        {
            struct Widget
            {
                int i;
                int j;
            };
            
            // Widget widget(1, 2); (ЕЩЕ НЕ СДЕЛАЛИ в C++23)
        }
    }
    // Порядок инициализации членов класса зависит от порядка их объявления, а не от порядка их в конструкторе!!!
    {
        initialization::start();
    }
    std::cout << std::endl;
    // Обычное наследование
    {
        /*
         Общая таблица спецификаторов доступа и типов наследования:
         В родительском классе | public в дочернем классе | private в дочернем классе | protected в дочернем классе
         public                | public                   | private                   | protected
         private               | Недоступен               | Недоступен                | Недоступен
         protected             | protected                | private                   | protected
         */
        
        inheritance::start();
    }
    std::cout << std::endl;
    // Наследование
    {
        inheritance::start();
    }
    /*
     Выравнивание памяти (aligment) - необходимо для эффективного обращения процессора к данным в памяти. 
     Процессору проще обращаться к данным, когда они последовательно лежат по блокам в 4 байта для 32-битной ОС /8 байт 64-битной ОС, поэтому компилятор подкладывает (padding) неиспользуемые байты для выравнивания к границам блоков памяти по 4/8 байт.
     */
    {
        aligment::start();
    }
    /*
     POD (plain old data) - простая структура данных (std::is_pod), занимающая непрерывную область памяти, компилятор НЕ оптимизируют поля класса/структуры: они находятся в памяти в том порядке, в котором они указаны (возможно с некоторым выравниванием - aligment), поэтому объекты такого типа можно скопировать с помощью memcpy, сериализировать по сети и воссоздать. Противоположность POD типа — управляемая структура данных, которую компилятор может оптимизировать поля класса/структуры по усмотрению (переставить местами), такая перестановка может серьёзно сэкономить память, но нарушает совместимость. Объекты POD быстрее создаются и копируются, чем объекты управляемой структуры данных.
     C++20: POD типа уже не будет, останутся только тривиальный тип и тип со стандартным устройством.
     Имеет характеристики: тривиального класса/структуры (trivial type) + со стандартным устройством (standard layout).
     
     Выравнивание памяти (aligment) - необходимо для эффективного обращения процессора к данным в памяти. Процессору проще обращаться к данным, когда они последовательно лежат по блокам в 4 байта для 32-битной ОС /8 байт 64-битной ОС, поэтому компилятор подкладывает (padding) неиспользуемые байты для выравнивания к границам блоков памяти по 4/8 байт.
     
     Тривиальный класс/структура (std::is_trivial) - занимает непрерывную область памяти, поэтому компилятор может самостоятельно выбирать способ упорядочивания членов
     Характеристики:
     — конструктор: явно отсутствующий или явно задан как default.
     — копирующий конструктор: явно отсутствующий или явно задан как default.
     — перемещающий конструктор: явно отсутствующий или явно задан как default.
     — копирующий оператор присваивания: явно отсутствующий или явно задан как default.
     — перемещающий оператор присваивания: явно отсутствующий или явно задан как default.
     — деструктор: явно отсутствующий или явно задан как default.
     - все поля и базовые классы — тоже тривиальные типы.
     - все поля класса - не инициализированны.
     - поля могут иметь разный модификатор доступа (public/protected/private).
     - не имеет виртуальных методов (включая деструктор).
     - не имеет виртуальных базовых типов (класс/структура).
     
     Класс/структура со стандартным устройством (std::is_standard_layout).
     Характеристики:
     - не имеет виртуальных методов.
     - не имеет виртуальных базовых типов (класс/структура).
     - не имеет полей-ссылок.
     - все поля имеют одинаковый модификатор доступа (public/protected/private).
     - нельзя наследоваться от наследуемого класса/структуры.
     - все поля и базовые классы — тоже типы со стандартным устройством.
     */
    std::cout << std::endl;
    {
        using namespace POD;
        std::cout << "POD" << std::endl;
        
        start();
    }
    std::cout << std::endl;
    // Задачи в интервью
    {
        interview::start();
    }
}
