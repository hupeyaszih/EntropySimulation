#ifndef MAP_H
#define MAP_H

#include "globals.h"
#include <string>
#include <unordered_map>
#include <vector>

enum OBJECT_TYPE{
    A,
    B,
    C,
    D,
    COUNT
};

struct Object{
    int id;
    int mass;
    int x,y;
    int entropyResistance;
    float vx,vy; //velocity
    OBJECT_TYPE type;
};

struct Cell {
    Object* occupant;
};

class Map{
private:
    Cell* grid;
    std::unordered_map<std::string, int> parameters;
public:
    Map();
    void Init();
    Cell& getCell(int x,int y);

    void addParameter(OBJECT_TYPE type, std::string parameterName,int parameter);
    int getParameter(OBJECT_TYPE type, std::string parameterName);
    
    std::vector<Object> objects;
};

#endif