#include "Aligment.hpp"

#include <iostream>

namespace aligment
{
    template<typename ...Args>
    struct alignas(Args...) Padding
    {
        char buffer[std::max({sizeof(Args)... })];
    };

    void Start()
    {
        /*
         Выравнивание памяти (aligment) - необходимо для эффективного обращения процессора к данным в памяти. Процессору проще обращаться к данным, когда они последовательно лежат по блокам в 4 байта для 32-битной ОС /8 байт 64-битной ОС, поэтому компилятор подкладывает (padding) неиспользуемые байты для выравнивания к границам блоков памяти по 4/8 байт.
         */
        {
            // 1 Способ: инициализация членов в случайном порядке
            {
                struct Padding // Должно быть bytes: 11
                {
                    char c1;     // bytes: 1
                    int number1; // bytes: 4
                    char c2;     // bytes: 1
                    char c3;     // bytes: 1
                    int number2; // bytes: 4
                };
                
                [[maybe_unused]] auto padding_size = sizeof(Padding); // На самом деле bytes: 16 - размер структуры
                [[maybe_unused]] auto padding_align = alignof(Padding); // bytes: 4 - выравнивание по границе
                [[maybe_unused]] auto c1_offset = offsetof(Padding, c1); // offset: 0
                [[maybe_unused]] auto number1_offset = offsetof(Padding, number1); // offset: 4
                [[maybe_unused]] auto c2_offset = offsetof(Padding, c2); // offset: 8
                [[maybe_unused]] auto c3_offset = offsetof(Padding, c3); // offset: 9
                [[maybe_unused]] auto number2_offset = offsetof(Padding, number2); // offset: 12
                
                /*
                 Хранение Padding в блоках памяти по 4 байта:
                 1      |4      |1,1      |4
                 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
                 */
            }
            // 2 Способ: инициализация членов в порядке их типа
            {
                // 1 Способ: обычный
                {
                    struct Padding // Должно быть bytes: 11
                    {
                        char c1;     // bytes: 1
                        char c2;     // bytes: 1
                        char c3;     // bytes: 1
                        int number1; // bytes: 4
                        int number2; // bytes: 4
                    };
                    
                    [[maybe_unused]] auto padding_size = sizeof(Padding); // На самом деле bytes: 12 - размер структуры
                    [[maybe_unused]] auto padding_align = alignof(Padding); // bytes: 4 - выравнивание по границе
                    [[maybe_unused]] auto c1_offset = offsetof(Padding, c1); // offset: 0
                    [[maybe_unused]] auto c2_offset = offsetof(Padding, c2); // offset: 1
                    [[maybe_unused]] auto c3_offset = offsetof(Padding, c3); // offset: 2
                    [[maybe_unused]] auto number1_offset = offsetof(Padding, number1); // offset: 4
                    [[maybe_unused]] auto number2_offset = offsetof(Padding, number2); // offset: 8
                    
                    /*
                     Хранение Padding в блоках памяти по 4 байта:
                     1,1,   |4      |4
                     0,1,2,3,4,5,6,7,8,9,10,11
                     */
                }
                // 2 Способ: variadic template: Padding будет выровнен по адресу std::max(int,char)
                {
                    [[maybe_unused]] Padding<int, char> padding; // Должно быть bytes: 11
                    
                    [[maybe_unused]] auto padding_size = sizeof(Padding<int, char>); // На самом деле bytes: 4 - размер структуры
                    [[maybe_unused]] auto padding_align = alignof(Padding<int, char>); // bytes: 4 - выравнивание по границе
                }
            }
            // 3 Способ: инициализация членов в обратном порядке их типа
            {
                struct Padding // Должно быть bytes: 11
                {
                    int number1; // bytes: 4
                    int number2; // bytes: 4
                    char c1;     // bytes: 1
                    char c2;     // bytes: 1
                    char c3;     // bytes: 1
                };
                
                [[maybe_unused]] auto padding_size = sizeof(Padding); // На самом деле bytes: 12 - размер структуры
                [[maybe_unused]] auto padding_align = alignof(Padding); // bytes: 4 - выравнивание по границе
                [[maybe_unused]] auto number1_offset = offsetof(Padding, number1); // offset: 0
                [[maybe_unused]] auto number2_offset = offsetof(Padding, number2); // offset: 4
                [[maybe_unused]] auto c1_offset = offsetof(Padding, c1); // offset: 8
                [[maybe_unused]] auto c2_offset = offsetof(Padding, c2); // offset: 9
                [[maybe_unused]] auto c3_offset = offsetof(Padding, c3); // offset: 10
                
                /*
                 Хранение Padding в блоках памяти по 4 байта:
                 4      |4      |1,1,1
                 0,1,2,3,4,5,6,7,8,9,10,11
                 */
            }
            // 4 Способ: добавление типа char для выравнивания первого блока памяти из 4 байт: 3(char) + 1(char) = 4 байта
            {
                struct Padding // Должно быть bytes: 11
                {
                    char c1;     // bytes: 1
                    char c2;     // bytes: 1
                    char c3;     // bytes: 1
                    char c4;     // bytes: 1
                    int number1; // bytes: 4
                    int number2; // bytes: 4
                };
                
                [[maybe_unused]] auto padding_size = sizeof(Padding); // На самом деле bytes: 12 - размер структуры
                [[maybe_unused]] auto padding_align = alignof(Padding); // bytes: 4 - выравнивание по границе
                [[maybe_unused]] auto c1_offset = offsetof(Padding, c1); // offset: 0
                [[maybe_unused]] auto c2_offset = offsetof(Padding, c2); // offset: 1
                [[maybe_unused]] auto c3_offset = offsetof(Padding, c3); // offset: 2
                [[maybe_unused]] auto c4_offset = offsetof(Padding, c3); // offset: 3
                [[maybe_unused]] auto number1_offset = offsetof(Padding, number1); // offset: 4
                [[maybe_unused]] auto number2_offset = offsetof(Padding, number2); // offset: 8
                
                /*
                 Хранение Padding в блоках памяти по 4 байта:
                 1,1,1,1|4      |4
                 0,1,2,3,4,5,6,7,8,9,10,11
                 */
            }
            // 5 Способ: выравнивание происходит по границе 8 байт из-за самого тяжеловесного типу double
            {
                struct Padding // Должно быть bytes: 15
                {
                    char c1;     // bytes: 1
                    double flag; // bytes: 8
                    char c2;     // bytes: 1
                    char c3;     // bytes: 1
                    int number;  // bytes: 4
                };
                
                [[maybe_unused]] auto padding_size = sizeof(Padding); // На самом деле bytes: 24 - размер структуры
                [[maybe_unused]] auto padding_align = alignof(Padding); // bytes: 8 - выравнивание по границе
                [[maybe_unused]] auto c1_offset = offsetof(Padding, c1); // bytes: 0 - выравнивание по границе
                [[maybe_unused]] auto flag_offset = offsetof(Padding, flag); // offset: 8
                [[maybe_unused]] auto c2_offset = offsetof(Padding, c2); // offset: 16
                [[maybe_unused]] auto c3_offset = offsetof(Padding, c3); // offset: 17
                [[maybe_unused]] auto number_offset = offsetof(Padding, number); // offset: 20
                
                /*
                 Хранение Padding в блоках памяти по 4 байта:
                 1              |8                    |1, 1       |4
                 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23
                 */
            }
            // 6 Способ: выравнивание при наследовании == выравнивание сложных объектов
            {
                // Раннее связывание - статический полиморфизм
                {
                    struct Base // Должно быть bytes: 5
                    {
                        int number; // bytes: 4
                        char c;     // bytes: 1
                    };
                    
                    [[maybe_unused]] auto base_padding_size = sizeof(Base); // На самом деле bytes: 8 - размер структуры
                    [[maybe_unused]] auto base_padding_align = alignof(Base); // bytes: 4 - выравнивание по границе
                    [[maybe_unused]] auto base_number_offset = offsetof(Base, number); // offset: 0
                    [[maybe_unused]] auto base_c_offset = offsetof(Base, c); // // offset: 4
                    
                    /*
                     Хранение Padding в блоках памяти по 4 байта:
                     4      |1
                     0,1,2,3,4,5,6,7
                     */
                    
                    // Выравнивание при наследовании
                    struct Derived : Base // Должно быть bytes: 6
                    {
                        char c; // bytes: 1
                    };
                    
                    [[maybe_unused]] auto derived_padding_size = sizeof(Derived); // На самом деле bytes: 8 - размер структуры
                    [[maybe_unused]] auto derived_padding_align = alignof(Derived); // bytes: 4 - выравнивание по границе
                    [[maybe_unused]] auto derived_c_offset = offsetof(Derived, c); // offset: 8
                    
                    /*
                     Хранение Padding в блоках памяти по 4 байта:
                     4      |4      |1
                     0,1,2,3,4,5,6,7,8,9,10,11
                     */
                    
                    // Выравнивание сложных объектов
                    struct Example // Должно быть bytes: 11
                    {
                        Base base; // bytes: 8
                        char c;     // bytes: 1
                    };
                    
                    [[maybe_unused]] auto example_padding_size = sizeof(Example); // На самом деле bytes: 12 - размер структуры
                    [[maybe_unused]] auto example_padding_align = alignof(Example); // bytes: 4 - выравнивание по границе
                    [[maybe_unused]] auto example_base_offset = offsetof(Example, base); // offset: 0
                    [[maybe_unused]] auto example_c_offset = offsetof(Example, c); // offset: 8
     
                    [[maybe_unused]] auto compare = sizeof(Derived) == sizeof(Example);
                    
                    /*
                     Хранение Padding в блоках памяти по 4 байта:
                     4      |4      |1
                     0,1,2,3,4,5,6,7,8,9,10,11
                     */
                }
                // Позднее связывание - динамический полиморфизм,
                {
                    struct Base // Должно быть bytes: 4
                    {
                        virtual void function() = 0;
                        virtual ~Base() = default;
                        int number; // bytes: 4
                    };
                    
                    [[maybe_unused]] auto base_padding_size = sizeof(Base); // На самом деле bytes: 16 - размер структуры
                    [[maybe_unused]] auto base_padding_align = alignof(Base); // bytes: 8 - выравнивание по границе
                    [[maybe_unused]] auto base_number_offset = offsetof(Base, number); // offset: 8
                    
                    /*
                     Хранение Padding в блоках памяти по 8 байтов:
                     8              |4
                     0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
                     */
                    
                    // Выравнивание при наследовании
                    struct Derived1 : Base // Должно быть bytes: 5
                    {
                        char c; // bytes: 1
                        void function() override {};
                    };
                    
                    // Выравнивание при наследовании
                    struct Derived2 : Base // Должно быть bytes: 5
                    {
                        char c; // bytes: 1
                        void function() override {};
                    };
                    
                    [[maybe_unused]] auto derived_padding_size = sizeof(Derived1); // На самом деле bytes: 16 - размер структуры
                    [[maybe_unused]] auto derived_padding_align = alignof(Derived1); // bytes: 8 - выравнивание по границе
                    [[maybe_unused]] auto derived_c_offset = offsetof(Derived1, c); // offset: 12
                    
                    /*
                     Хранение Padding в блоках памяти по 8 байтов:
                     8              |4        |1
                     0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
                     */
                    
                    // Приводит первые 8 байтов к указателю vpointer
                    auto PrintVPointer = [](void* object) -> void
                    {
                        auto pointer = reinterpret_cast<size_t*>(object);
                        std::cout << "VPointer: " << *pointer << std::endl;
                    };
                    
                    Derived1 derived11, derived12;
                    
                    std::cout << "VPointer один для экземпляров класса Derived1: " << std::endl;
                    PrintVPointer(&derived11);
                    PrintVPointer(&derived12);
                    
                    std::cout << "VPointer один для экземпляров класса Derived2: " << std::endl;
                    Derived2 derived21, derived22;
                    PrintVPointer(&derived21);
                    PrintVPointer(&derived22);
                }
            }
            /*
             7 Способ: Современные процессоры не загружают данные по байту из памяти, обычно по 4 байта сразу: если требуемые данные не выравнены по 4 байтам, то процессору приходится производить разбиение для записи в кэш, а при чтении объединять требуемые байты, что приводит к уменьшению производительности ЦП при чтении до 3-х раз.
             C++17: спецификатор alignas - позволяющий вручную указать компилятору границу выравнивание для типа или переменой (2,4,8,16,32 байта) - это помогает уменьшить число разбиений данных для хранения кэшах и объединений при чтении данных ЦП.
             Плюсы:
             - увеличение производительности за счет сокращения циклов ЦП при чтении.
             - избежание лишних подкладываний (padding) в строках кэша.
             Минусы:
             - выделение больше памяти.
             */
            {
                struct alignas(16) Padding // Должно быть bytes: 11
                {
                    int number1; // bytes: 4
                    char c1;     // bytes: 1
                    char c2;     // bytes: 1
                    char c3;     // bytes: 1
                    int number2; // bytes: 4
                    int number3; // bytes: 4
                };
                
                [[maybe_unused]] auto padding_size = sizeof(Padding); // На самом деле bytes: 16 - размер структуры
                [[maybe_unused]] auto padding_align = alignof(Padding); // bytes: 16 - выравнивание по границе
                [[maybe_unused]] auto number1_offset = offsetof(Padding, number1); // offset: 0
                [[maybe_unused]] auto c1_offset = offsetof(Padding, c1); // offset: 4
                [[maybe_unused]] auto c2_offset = offsetof(Padding, c2); // offset: 5
                [[maybe_unused]] auto c3_offset = offsetof(Padding, c3); // offset: 6
                [[maybe_unused]] auto number2_offset = offsetof(Padding, number2); // offset: 8
                [[maybe_unused]] auto number3_offset = offsetof(Padding, number3); // offset: 12
                
                /*
                 Хранение Padding в блоках памяти по 16 байт:
                 4       1,1,1   4         4          |
                 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
                 */
            }
            /*
             8 Способ: отключение/изменение выравнивания с помощью макроса #pragma pack (push, 1), 1 - устанавливает размер выравнивания в байтах.
             Плюсы: уменьшение потребляемой памяти.
             Минусы: уменьшение скорости работы.
             */
            {
#pragma pack (push, 1)
                struct Padding // Должно быть bytes: 15
                {
                    char c1;     // bytes: 1
                    double flag; // bytes: 8
                    char c2;     // bytes: 1
                    char c3;     // bytes: 1
                    int number;  // bytes: 4
                };
#pragma pack (pop)
                
                [[maybe_unused]] auto padding_size = sizeof(Padding); // На самом деле bytes: 15 - размер структуры
                [[maybe_unused]] auto padding_align = alignof(Padding); // bytes: 1 - выравнивание по границе
                [[maybe_unused]] auto c1_offset = offsetof(Padding, c1); // bytes: 0 - выравнивание по границе
                [[maybe_unused]] auto flag_offset = offsetof(Padding, flag); // offset: 1
                [[maybe_unused]] auto c2_offset = offsetof(Padding, c2); // offset: 9
                [[maybe_unused]] auto c3_offset = offsetof(Padding, c3); // offset: 10
                [[maybe_unused]] auto number_offset = offsetof(Padding, number); // offset: 11
                
                /*
                 Хранение Padding в блоках памяти по 1 байту:
                 1|8              |1|1 |4
                 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
                 */
            }
            /*
             9 Способ: плотная упаковка полей - опуститься до уровней битов.
             Плюсы: уменьшение потребляемой памяти.
             Минусы: уменьшение скорости работы.
             */
            {
                // 1 Способ: битовые поля
                {
                    struct Padding // Должно быть bytes: 4
                    {
                        uint16_t number1:10; // bits: 10
                        uint8_t c1:4;        // bits: 4
                        uint8_t c2:4;        // bits: 4
                        uint8_t c3:4;        // bits: 4
                        uint16_t number2:10; // bits: 10
                    };
                    
                    [[maybe_unused]] auto padding_size = sizeof(Padding); // На самом деле bytes: 6 - размер структуры
                    [[maybe_unused]] auto padding_align = alignof(Padding); // bytes: 2 - выравнивание по границе
                    
                    /*
                     c1 + c2 уйдут в 1 бит, с3 займет 0,5 бита -> c1 + c2 + c3 = 1,5 байта
                     Хранение Padding в блоках памяти по 2 байта:
                     2  |1,5|2
                     0,1,2,3,4,5
                     */
                }
                // 2 Способ: подогнать размер все полей структуры в один тип
                {
                    struct Padding // Должно быть bytes: 4
                    {
                        uint32_t number1:10; // bits: 10
                        uint32_t c1:4;       // bits: 4
                        uint32_t c2:4;       // bits: 4
                        uint32_t c3:4;       // bits: 4
                        uint32_t number2:10; // bits: 10
                    };
                    
                    [[maybe_unused]] auto padding_size = sizeof(Padding); // На самом деле bytes: 4 - размер структуры
                    [[maybe_unused]] auto padding_align = alignof(Padding); // bytes: 2 - выравнивание по границе
                    
                    static_assert(sizeof(Padding) == 4, "Wrong message!");
    
                    /*
                     number1 + c1 + c2 + c3 + number2 = 4 байта
                     Хранение Padding в блоках памяти по 4 байта:
                     4
                     0,1,2,3
                     */
                    
                    /*
                     number1 + c1 + c2 + c3 + number2 = 32 бита
                     Хранение Padding в блоках памяти по 32 байта:
                     number1            |c1         |c2         |c3         |number2
                     0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32
                     */
                }
            }
            
            std::cout << std::endl;
        }
    }
}
