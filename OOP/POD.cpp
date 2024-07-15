#include "POD.hpp"

#include <iostream>
#include <type_traits>

#define _SILENCE_CXX20_IS_POD_DEPRECATION_WARNING 


/*
 Видео: https://www.youtube.com/watch?v=KqqrJYEUeTw&ab_channel=cppProsto
 */

/*
 Сайты: https://habr.com/ru/articles/470265/
        https://learn.microsoft.com/ru-ru/cpp/cpp/trivial-standard-layout-and-pod-types?view=msvc-170
        https://habr.com/ru/articles/532972/
        https://ru.wikipedia.org/wiki/%D0%9F%D1%80%D0%BE%D1%81%D1%82%D0%B0%D1%8F_%D1%81%D1%82%D1%80%D1%83%D0%BA%D1%82%D1%83%D1%80%D0%B0_%D0%B4%D0%B0%D0%BD%D0%BD%D1%8B%D1%85
 
 */


namespace POD
{
    namespace trivial_type
    {
        struct T1{};
        
        class T2
        {
        public:
            int i;
        protected:
            int j;
        private:
            int z;
        };

        struct T3
        {
            T3() = default;
            ~T3() = default;
            T3(const T3&) = default;
            T3(T3&&) = default;
            T3& operator=(const T3&) = default;
            T3& operator=(T3&&) = default;
        };

        class T4
        {
        public:
            T4() = default;
            T4(int number){ _number = number; }
         
        protected:
            int _number;
        };

        struct T5
        {
            static int number;
        };

        /// Есть инициализация члена по умолчанию
        class T6
        {
        private:
            int a = 0;
        };

        /// Определен явно пользовательский конструктор
        class T7
        {
        public:
            T7(){};
         
        protected:
            int a;
        };

        /// Определен виртуальный деструктор
        class T8
        {
            virtual ~T8() = default;
        };
    }

    namespace standard_layout
    {
        struct S1 {};

        class S2
        {
        private:
            int z;
        };

        struct S3
        {
            S3() = default;
            ~S3() = default;
            S3(const S3&) = default;
            S3(S3&&) = default;
            S3& operator=(const S3&) = default;
            S3& operator=(S3&&) = default;
        };

        class S4
        {
        public:
            S4() = default;
            S4(int number){ _number = number; }
         
        protected:
            int _number;
        };

        class S5
        {
        private:
            int _number = 0;
        };

        class S6
        {
        public:
            S6(){};
         
        protected:
            int a;
        };

        struct S7
        {
            static int number;
        };

        /// Есть разные модификатора доступа
        class S8
        {
        public:
            int i;
        protected:
            int j;
        private:
            int z;
        };

        /// Определен виртуальный деструктор
        class S9
        {
            virtual ~S9() = default;
        };

        namespace inheritance
        {
            struct Derived1 : S1
            {};
        
            struct Derived2 : S1
            {
                S2 s2;
                S3 s3;
            };
        
            struct Derived3 : S1, S2
            {};
        
            /// Член класса такой же как и базовый класс
            class Derived4 : S1
            {
                S1 s1;
            };
        
            /// 2 места обьявления члена класса в S1, S2
            class Derived5 : S1, S2
            {
                int number;
            };

            struct A {};
            struct B : A {};
            struct C : A {};
            /// Нельзя наследоваться от наследуемого класса/структуры
            struct D : B, C {};
        }
    }

    void Start()
    {
        
        /*
         Тривиальный класс/структура (std::trivial type).
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
         */
        {
            using namespace trivial_type;
            std::cout << "trivial type" << std::endl;
            
            [[maybe_unused]] auto t1 = std::is_trivial<T1>::value; // true
            [[maybe_unused]] auto t2 = std::is_trivial<T2>::value; // true
            [[maybe_unused]] auto t3 = std::is_trivial<T3>::value; // true
            [[maybe_unused]] auto t4 = std::is_trivial<T4>::value; // true
            [[maybe_unused]] auto t5 = std::is_trivial<T5>::value; // false
            /// Есть инициализация члена по умолчанию
            [[maybe_unused]] auto t6 = std::is_trivial<T6>::value; // false
            /// Определен явно пользовательский конструктор
            [[maybe_unused]] auto t7 = std::is_trivial<T7>::value; // false
            /// Определен виртуальный деструктор
            [[maybe_unused]] auto t8 = std::is_trivial<T8>::value; // false
        }
        /*
         Класс/структура со стандартным устройством (std::standard layout).
         Характеристики:
         - не имеет виртуальных методов.
         - не имеет виртуальных базовых типов (класс/структура).
         - не имеет полей-ссылок.
         - все поля имеют одинаковый модификатор доступа (public/protected/private).
         - нельзя наследоваться от наследуемого класса/структуры.
         - все поля и базовые классы — тоже типы со стандартным устройством.
         */
        {
            using namespace standard_layout;
            std::cout << "standard layout" << std::endl;
            
            [[maybe_unused]] auto s1 = std::is_standard_layout<S1>::value; // true
            [[maybe_unused]] auto s2 = std::is_standard_layout<S2>::value; // true
            [[maybe_unused]] auto s3 = std::is_standard_layout<S3>::value; // true
            [[maybe_unused]] auto s4 = std::is_standard_layout<S4>::value; // true
            [[maybe_unused]] auto s5 = std::is_standard_layout<S5>::value; // true
            [[maybe_unused]] auto s6 = std::is_standard_layout<S6>::value; // true
            [[maybe_unused]] auto s7 = std::is_standard_layout<S7>::value; // false
            
            /// Есть разные модификатора доступа
            [[maybe_unused]] auto s8 = std::is_standard_layout<S8>::value; // false
            /// Определен виртуальный деструктор
            [[maybe_unused]] auto s9 = std::is_standard_layout<S9>::value; // false
            
            /// Наследование
            {
                using namespace inheritance;
                std::cout << "inheritance" << std::endl;
                
                [[maybe_unused]] auto derived1 = std::is_standard_layout<Derived1>::value; // true
                [[maybe_unused]] auto derived2 = std::is_standard_layout<Derived2>::value; // true
                [[maybe_unused]] auto derived3 = std::is_standard_layout<Derived3>::value; // true
                /// Член класса такой же как и базовый класс
                [[maybe_unused]] auto derived4 = std::is_standard_layout<Derived4>::value; // false
                /// 2 места обьявления члена класса в S1, S2
                [[maybe_unused]] auto derived5 = std::is_standard_layout<Derived5>::value; // false
                
                [[maybe_unused]] auto a = std::is_standard_layout<A>::value; // true
                [[maybe_unused]] auto b = std::is_standard_layout<B>::value; // true
                [[maybe_unused]] auto c = std::is_standard_layout<C>::value; // true
                /// Нельзя наследоваться от наследуемого класса/структуры
                [[maybe_unused]] auto d = std::is_standard_layout<D>::value; // false
                
                std::cout << std::endl;
            }
        }
        /*
         POD (plain old data) - простая структура данных (std::is_pod), занимающая непрерывную область памяти, компилятор НЕ оптимизируют поля класса/структуры: они находятся в памяти в том порядке, в котором они указаны (возможно с некоторым выравниванием), поэтому объекты такого типа можно скопировать с помощью memcpy, сериализировать по сети и воссоздать. Противоположность POD типа — управляемая структура данных, которую компилятор может оптимизировать поля класса/структуры по усмотрению (переставить местами), такая перестановка может серьёзно сэкономить память, но нарушает совместимость. Объекты POD быстрее создаются и копируются, чем объекты управляемой структуры данных.
         C++20: POD типа уже не будет, останутся только тривиальный тип и тип со стандартным устройством.
         Имеет характеристики: тривиального класса/структуры (trivial type) + со стандартным устройством (standard layout).
         */
        {
            std::cout << "POD" << std::endl;
            
            using namespace trivial_type;
            
            [[maybe_unused]] auto t1 = std::is_pod<T1>::value; // true
            /// Есть разные модификатора доступа
            [[maybe_unused]] auto t2 = std::is_pod<T2>::value; // false
            [[maybe_unused]] auto t3 = std::is_pod<T3>::value; // true
            [[maybe_unused]] auto t4 = std::is_pod<T4>::value; // true
            
            using namespace standard_layout;
            
            [[maybe_unused]] auto s1 = std::is_pod<S1>::value; // true
            [[maybe_unused]] auto s2 = std::is_pod<S2>::value; // true
            [[maybe_unused]] auto s3 = std::is_pod<S3>::value; // true
            [[maybe_unused]] auto s4 = std::is_pod<S4>::value; // true
            /// Есть инициализация члена по умолчанию
            [[maybe_unused]] auto s5 = std::is_pod<S5>::value; // false
            /// Определен явно пользовательский конструктор
            [[maybe_unused]] auto s6 = std::is_pod<S6>::value; // false
            
            using namespace inheritance;
            
            [[maybe_unused]] auto derived1 = std::is_pod<Derived1>::value; // true
            [[maybe_unused]] auto derived2 = std::is_pod<Derived2>::value; // true
            [[maybe_unused]] auto derived3 = std::is_pod<Derived3>::value; // true
            
            [[maybe_unused]] auto a = std::is_pod<A>::value; // true
            [[maybe_unused]] auto b = std::is_pod<B>::value; // true
            [[maybe_unused]] auto c = std::is_pod<C>::value; // true
            
            std::cout << std::endl;
        }
        
        std::cout << std::endl;
    }
}
