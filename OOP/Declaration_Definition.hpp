#ifndef Declaration_Definition_h
#define Declaration_Definition_h

#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

/*
 Лекция: https://www.youtube.com/watch?v=G_jcBrrYPAs&t=3134s&pp=ygUXc3RhdGljIGV4dGVybiBjb25zdCBjKys%3D       https://www.youtube.com/watch?v=c7CasTJKw7o&list=PLlb7e2G7aSpTFea2FYxp7mFfbZW-xavhL&index=11&ab_channel=ComputerScienceCenter
 Сайты: https://habr.com/ru/companies/otus/articles/432834/
        https://habr.com/ru/companies/jugru/articles/506104/
 */

/*
Внутреннее связывание/компоновка/линковка internal linkage) - использование функции/переменной в ОДНОЙ единице трансляции (.cpp, включающий содержимое всех .h). При объявлении символа с внутренним связыванием в заголовочном файле (.hpp), каждая единица трансляции (.cpp), включающая в себя этот файл, получит копию этого символа - это плохо, т.к. расход памяти для больших объектов может быть очень высок из-за копирования под каждую единицу трансляции при формирования промежуточных объектных файлов (.o - Linux/.obj - Windows).
Примеры:
- const переменная. Исключения: extern const, inline const - имеют внешнее связывание (external linkage)
- constexpr переменная (имеет свойства const)
- static переменная/функция
- безымянный namespace (все элементы находящиеся внутри, даже external linkage, получают internal linkage)
 */
namespace internal_linkage
{
    /*
     int number = 5; // Не скомпилируется, потому что это внешнее свяязывание (external linkage) - в каждой единице трансляции будет свое определение переменной.
     Нужно сделать static, const, constexpr:
     */
    static int number1 = 1;
    const int number2 = 2;
    constexpr int number3 = 3;
    static const int number4 = 4;
    static constexpr int number5 = 5;
    static inline int number6 = 6; // Так лучше не писать: inline не отменяет internal linkage от static

    /*
     const char* error = "error"; buffer — это указатель на константу, но не является константным указателем, имеет внешнее свяязывание (external linkage) - в каждой единице трансляции будет свое определение переменной.
     Нужно сделать const:
    */
    const char* const buffer = "buffer";

    /*
    void function()
    {
        std::cout << "print" << std::endl;
    }
    Не скомпилируется, потому что функция имеет внешнее связывание (external linkage) - в каждой единице трансляции будет свое определение функции.
     Нужно сделать static:
    */

    static void static_function()
    {
        std::cout << "static function" << std::endl;
    }

    namespace print
    {
        int& Number1();
        const int& Number2();
        const int& Number3();
        const int& Number4();
        const int& Number5();
        int& Number6();
    
        using ptr = void(*)();
        ptr Static_Function();
    
    }
}

/*
Внешнее связывание/комповнока/линковка (external linkage) - использование функции/переменной в разных единицах трансляции (.cpp, включающий содержимое всех .h).
Примеры:
- enum
- extern переменная/функция, если не указано static
- inline переменная/функция, если не указано static
- глобальные переменные без const и static.
- обычная функция без static (внутри все элементы не имеют связывания, вне зависимости external linkage или internal linkage)
- constexpr функция (имеет свойства inline)
- шаблоны
- класс/структура имеют external linkage, как и их static и constexpr члены и методы, а не static переменные-члены — не имеют linkage (no linkage).
- метод класса/структуры
- именнованные namespace, чтобы одно пространство имён можно было использовать в разных единицах трансляции. Но это не влияет на linkage переменных внутри него — они могут иметь как internal, так и external linkage.
 */
namespace external_linkage
{
    /*
     int number = 5; // Не скомпилируется, потому что в каждой единице трансляции будет свое определение переменной.
     Нужно сделать extern или inline:
     */
    extern int number1;
    inline int number2 = 10;
    inline constexpr int number3 = 10;       // inline отменяет internal linkage от constexpr
    inline const std::string str = "string"; // inline отменяет internal linkage от const
    /*
     Это не скомпилируется, потому что в каждой единице трансляции будет свое определение функции.
     Нужно сделать inline, constexpr (имеет свойства inline):
     int sqrt(int x)
     {
         return x * x;
     }
     */
    inline int sqrt2(int x)
    {
        return x * x;
    }

    constexpr int sqrt1(int x)
    {
        return x * x;
    }

    struct Example2
    {
        // int nummber = 0; // no linkage: обычные переменные-члены класса/структуры не имеют связывания
        static int number1;
        static const int number2;
        static constexpr int number3 = 3; // C++17
        static inline int number4 = 4; // C++17
        
        /*
         Не имеет смысла, т.к. constexpr = inline
         static inline constexpr int number5 = 5;
         
         Не имеет смысла, т.к. constexpr = const
         static constexpr const int number6 = 6;
         */
        
        static constexpr const char* str1 = "string"; // const char* - указатель на память константного символа
        static inline const std::string str2 = "string"; // так лучше не делать, а использовать std::string_view
        static constexpr std::string_view str3 = "string";
        
        /*
         constexpr всегда относится только ко всей спецификации типа со всеми модификаторами.
         
         static constexpr char* str4 = "string"; constexpr char* - константный указатель времени компиляции на память символа.
         */
        
        static void static_function()
        {
            std::cout << "static function" << std::endl;
        }
    };

    // inline constexpr шаблон переменной — можно определять в .h и использовать в разных .cpp.
    template <class T>
    inline constexpr bool is_const_v = std::is_const<T>::value;

    // Шаблоны функций можно определять без inline, поскольку ODR разрешает иметь одинаковые определения шаблона в разных единицах трансляции.
    template <class T>
    /*inline*/ void swap(T& lhs, T& rhs)
    {
        T temp = std::move(lhs);
        lhs = std::move(rhs);
        rhs = std::move(temp);
    }

    // inline для явной специализации имеет смысл, потому что полная специализация шаблона функции уже считается обычной функцией с точки зрения ODR
    template<>
    inline void swap(double& lhs, double& rhs)
    {
        double temp = std::move(lhs);
        lhs = std::move(rhs);
        rhs = std::move(temp);
    }

    namespace print
    {
        int& Number1();
        const int& Number2();
        const int& Number3();
        int& Number4();

        using ptr = void(*)();
        ptr Static_Function();
    }
}

#endif /* Declaration_Definition_h */
