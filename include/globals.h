#ifndef GLOBALS_H
#define GLOBALS_H

#define DEBUG_MODE

#ifdef DEBUG_MODE
#include <iostream>
#define LOG(x) std::cout << "[LOG] " << x << std::endl
#else
#define LOG(x)
#endif


extern int MAP_SIZE_X, MAP_SIZE_Y;

#endif