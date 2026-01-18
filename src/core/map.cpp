#include "core/map.h"
#include <cstdlib>
#include <string>

Map::Map() {

}

Cell& Map::getCell(int x,int y){
    return grid[y * MAP_SIZE_X + x]; // row-major
}

void Map::Init() {
    addParameter(OBJECT_TYPE::A, "mass", 1);
    addParameter(OBJECT_TYPE::A, "entropyResistance", 1);

    addParameter(OBJECT_TYPE::B, "mass", 3);
    addParameter(OBJECT_TYPE::B, "entropyResistance", 3);

    addParameter(OBJECT_TYPE::C, "mass", 6);
    addParameter(OBJECT_TYPE::C, "entropyResistance", 6);

    addParameter(OBJECT_TYPE::D, "mass", 2);
    addParameter(OBJECT_TYPE::D, "entropyResistance", 0);
    //////

    grid = new Cell[MAP_SIZE_X*MAP_SIZE_Y];
    objects.resize(MAP_SIZE_X*MAP_SIZE_Y);
    
    int type = 0;
    int id = 0;
    for(int y = 0;y < MAP_SIZE_Y;y++) {
        for(int x = 0;x < MAP_SIZE_X;x++) {
            
            OBJECT_TYPE choosenType = OBJECT_TYPE::A;
            switch (type%4) {
                case 0:
                    choosenType = OBJECT_TYPE::A;
                    break;
                case 1:
                    choosenType = OBJECT_TYPE::B;
                    break;
                case 2 :
                    choosenType = OBJECT_TYPE::C;
                    break;
                case 3:
                    choosenType = OBJECT_TYPE::D;
                    break;
            }
            
            objects[y*MAP_SIZE_X+x] = {id,getParameter(choosenType, "mass"),x,y,getParameter(choosenType, "entropyResistance"),1.0f,1.0f,choosenType};
            getCell(x, y).occupant = &objects[y*MAP_SIZE_X+x];
            id++;
            type++;    
        }
    }
    

}

void Map::addParameter(OBJECT_TYPE type, std::string parameterName,int parameter){
    std::string key = std::to_string(type) + parameterName;
    parameters[key] = parameter;
}

int Map::getParameter(OBJECT_TYPE type, std::string parameterName){
    std::string key = std::to_string(type) + parameterName;
    return parameters[key];
}