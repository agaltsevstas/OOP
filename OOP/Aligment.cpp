#include "Aligment.hpp"

#include <iostream>

namespace aligment
{
    void start()
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
                 Хранение Padding в памяти:
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
                 Хранение Padding в памяти:
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
            }
            // 6 Способ: выравнивание при наследовании == выравнивание сложных объектов
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
                
                // Выравнивание при наследовании
                struct Derived : Base // Должно быть bytes: 6
                {
                    char c; // bytes: 1
                };
                
                [[maybe_unused]] auto derived_padding_size = sizeof(Derived); // На самом деле bytes: 8 - размер структуры
                [[maybe_unused]] auto derived_padding_align = alignof(Derived); // bytes: 4 - выравнивание по границе
                [[maybe_unused]] auto derived_c_offset = offsetof(Derived, c); // offset: 8
                
                // Выравнивание сложных объектов
                struct Example // Должно быть bytes: 11
                {
                    Base base; // bytes: 8
                    char c;     // bytes: 1
                };
                
                [[maybe_unused]] auto example_padding_size = sizeof(Example); // На самом деле bytes: 20 - размер структуры
                [[maybe_unused]] auto example_padding_align = alignof(Example); // bytes: 4 - выравнивание по границе
                [[maybe_unused]] auto example_base_offset = offsetof(Example, base); // offset: 0
                [[maybe_unused]] auto example_c_offset = offsetof(Example, c); // offset: 8
 
                [[maybe_unused]] auto compare = sizeof(Derived) == sizeof(Example);
            }
            /*
             Современные процессоры не загружают данные по байту из памяти, обычно по 4 байта сразу: если требуемые данные не выравнены по 4 байтам, то процессору приходится производить разбиение для записи в кэш, а при чтении объединять требуемые байты, что приводит к уменьшению производительности ЦП при чтении до 3-х раз.
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
                 Хранение Padding в блоках памяти по 4 байта:
                 4      |1,1,1  |4        |4
                 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
                 */
            }
        }
    }
}
