#ifndef SIMULATION_H
#define SIMULATION_H

#include "core/map.h"
class Simulation{
public:
    Map map;
    Simulation();
    void Update();
    void Init();
};

#endif