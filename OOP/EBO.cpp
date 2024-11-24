#include "EBO.hpp"

#include <iostream>

namespace EBO
{
    void Start()
    {
        std::cout << "EBO" << std::endl;
        
        /*
         EBO (empty base optimization) - гарантирует размер любого объекта/подъекта должен быть не менее 1 байта, даже если тип является пустым, чтобы можно было получить разные адреса разных объектов одного и того же типа.
         */
        
        /// Без virtual
        {
            std::cout << "Без virtual" << std::endl;
            
            class Base { };
            class Derived : public Base {};
            
            std::cout << "Base: " <<  sizeof(Base) << std::endl;
            std::cout << "Derived: " <<  sizeof(Derived) << std::endl;
            
            std::cout << std::endl;
        }
        /// virtual: Указатель vtable/vpointer/vptr весит 4 байта 32-битного процессора / 8 байт 64-битного процессора, и сама таблица виртуальных функций занимает память.
        {
            std::cout << "virtual" << std::endl;
            
            class Base { void virtual print() {std::cout << "Base::print" << std::endl;} };
            class Derived : public Base { void print() override {std::cout << "Derived::print" << std::endl;} };
            
            std::cout << "Base: " <<  sizeof(Base) << std::endl;
            std::cout << "Derived: " <<  sizeof(Derived) << std::endl;
            
            std::cout << std::endl;
        }
    }
}
