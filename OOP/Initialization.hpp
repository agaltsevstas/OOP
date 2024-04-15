#ifndef Initialization_hpp
#define Initialization_hpp

#include <iostream>

/*
 Общая таблица спецификаторов доступа и типов наследования:
 В родительском классе | public в дочернем классе | private в дочернем классе | protected в дочернем классе
 public                | public                   | private                   | protected
 private               | Недоступен               | Недоступен                | Недоступен
 protected             | protected                | private                   | protected
 */

namespace initialization_order
{
    void start();
}

namespace inheritance
{
    void start();
}

#endif /* Initialization_h */
