#ifndef MAP_H
#define MAP_H

#include <vector>

enum OBJECT_TYPE{
    A,
    B,
    C,
    D
};

struct Object{
    int id;
    int mass;
    int x,y;
    int entropyResistance;

    OBJECT_TYPE type;
};

class Map{
public:
    Map();
    void Init();
    std::vector<Object> objects;
};

#endif