#ifndef PRINT_H
#define PRINT_H

// #define COMPILING_ON_ARDUINO

#ifdef COMPILING_ON_ARDUINO
    #include "Arduino.h"
    #define PRINT(thing) Serial.print()
#else
    #include <iostream>
    #define PRINT(thing) std::cout << (thing);
#endif 

#endif // PRINT_H