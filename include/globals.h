#ifndef GLOBALS_H
#define GLOBALS_H

#define DEBUG_MODE

#ifdef DEBUG_MODE
#include <iostream>
#define LOG(x) std::cout << "[LOG] " << x << std::endl;
#else
#define LOG(x)
#endif


#define MAP_SIZE_X 100
#define MAP_SIZE_Y 100

#endif