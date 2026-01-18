#ifndef SIMULATION_H
#define SIMULATION_H

#include "core/map.h"
struct Vector2D {
    int x;
    int y;
};

class Simulation{
public:
    Map* map;
    Simulation();
    ~Simulation();

    void Update();
    void Init();

    void move(Object& object, int dirX,int dirY);
    bool canSwap(const Object* o1, const Object* o2) const;

    float computeLocalEntropy(const Object& obj,int posX,int posY);

    Vector2D chooseDirectionFavoringOrder(const Object& obj); 
    Vector2D chooseDirectionFavoringEntropy(const Object& obj);    
    Vector2D chooseDirectionWithProtoGravity(const Object& obj);
};

#endif