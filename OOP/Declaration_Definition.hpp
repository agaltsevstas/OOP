#ifndef Declaration_Definition_h
#define Declaration_Definition_h

#include <string>
#include <type_traits>
#include <utility>

/*
 Лекция: https://www.youtube.com/watch?v=G_jcBrrYPAs&t=3134s&pp=ygUXc3RhdGljIGV4dGVybiBjb25zdCBjKys%3D       https://www.youtube.com/watch?v=c7CasTJKw7o&list=PLlb7e2G7aSpTFea2FYxp7mFfbZW-xavhL&index=11&ab_channel=ComputerScienceCenter
 Сайты: https://habr.com/ru/companies/otus/articles/432834/
        https://habr.com/ru/companies/jugru/articles/506104/
 */


/*
 Внутреннее связывание/компоновка/линковка (internal linkage) - использование функции/переменной в ОДНОЙ единице трансляции (.cpp, включающий содержимое всех .h). При объявлении символа с внутренним связыванием в заголовочном файле (.hpp), каждая единица трансляции (.cpp), включающая в себя этот файл, получит копию этого символа - это плохо, т.к. расход памяти для больших объектов может быть очень высок из-за копирования под каждую единицу трансляции при формирования промежуточных объектных файлов (.o - Linux/.obj - Windows).
   Пример:
   - const переменная
   - constexpr переменная, имеет свойства const
   - static
   - безымянный namespace
 */
namespace internal_linkage
{
    /*
     int number = 5; // Это не скомпилируется, потому что в каждой единице трансляции будет свое определение переменной
     Нужно сделать const, constexpr, inline:
     */
    static int number1 = 10;
    const int number2 = 10;
    constexpr int number3 = 10;

    /*
     const char* error = "error"; buffer — это указатель на константу, но не является константным указателем.
     Нужно сделать его константным:
    */
    const char* const buffer = "buffer";

    void print();
}

/*
 Внешнее связывание/комповнока/линковка (external linkage) - использование функции/переменной в разных единицах трансляции (.cpp, включающий содержимое всех .h).
   Пример:
   - extern
   - inline переменная/функция
   - Non-const глобальная переменная
   - функция
   - constexpr функция, имеет свойства inline
   - шаблоны
   - классы/структуры
   - именновынные namespace
 */
namespace external_linkage
{
    extern int number1;
    inline int number2 = 10;
    inline constexpr int number3 = 10;
    inline const std::string str = "string";

    void print();
    /*
     int sqrt(int x)
     {
         return x * x;
     }
     Это не скомпилируется, потому что в каждой единице трансляции будет свое определение функции.
     Нужно сделать constexpr:
     */
    constexpr int sqrt1(int x)
    {
        return x * x;
    }

    inline int sqrt2(int x)
    {
        return x * x;
    }

    struct Example2
    {
        static int number1;
        static const int number2;
        static constexpr int number3 = 3; // constexpr = inline + compile time + const
        static inline int number4 = 4;
        
        /*
         Не имеет смысла, т.к. constexpr = inline
         static inline constexpr int number5 = 5;
         
         Не имеет смысла, т.к. constexpr = const
         static constexpr const int number6 = 6;
         */
        static constexpr std::string_view str1 = "string";
        static inline const std::string str2 = "string"; // так лучше не делать, а использовать std::string_view
        
        // TODO
        static constexpr const char* str3 = "string"; // const char* - указатель на память константного символа
        
        /*
         constexpr всегда относится только ко всей спецификации типа со всеми модификаторами.
         
         static constexpr char* str4 = "string"; constexpr char* - константный указатель времени компиляции на память символа.
         */
    };

    //
    template <class T>
    inline constexpr bool is_const_v = std::is_const<T>::value;

    // inline для шаблонов функций бесполезен, потому что inline включен уже по-умолчанию (не всегда!).
    template <class T>
    /*inline*/ void swap(T& lhs, T& rhs)
    {
        T temp = std::move(lhs);
        lhs = std::move(rhs);
        rhs = std::move(temp);
    }

    // inline для явной специализации имеет смысл, потому что функция уже НЕшаблонная
    template<>
    inline void swap(double& lhs, double& rhs)
    {
        double temp = std::move(lhs);
        lhs = std::move(rhs);
        rhs = std::move(temp);
    }
}

#endif /* Declaration_Definition_h */
