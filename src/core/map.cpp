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

    // Type B → biraz daha ağır, orta direnç
    addParameter(OBJECT_TYPE::B, "mass", 2);
    addParameter(OBJECT_TYPE::B, "entropyResistance", 2);

    // Type C → ağır, orta direnç
    addParameter(OBJECT_TYPE::C, "mass", 4);
    addParameter(OBJECT_TYPE::C, "entropyResistance", 3);

    // Type D → hafif, yüksek direnç
    addParameter(OBJECT_TYPE::D, "mass", 1);
    addParameter(OBJECT_TYPE::D, "entropyResistance", 6);

    // Type E → orta ağırlık, düşük direnç
    addParameter(OBJECT_TYPE::E, "mass", 3);
    addParameter(OBJECT_TYPE::E, "entropyResistance", 1);

    // Type F → orta ağırlık, yüksek direnç
    addParameter(OBJECT_TYPE::F, "mass", 3);
    addParameter(OBJECT_TYPE::F, "entropyResistance", 7);

    // Type G → çok hafif, düşük direnç
    addParameter(OBJECT_TYPE::G, "mass", 1);
    addParameter(OBJECT_TYPE::G, "entropyResistance", 0);

    // Type H → çok ağır, orta direnç
    addParameter(OBJECT_TYPE::H, "mass", 6);
    addParameter(OBJECT_TYPE::H, "entropyResistance", 4);

    // Type I → hafif, orta direnç
    addParameter(OBJECT_TYPE::I, "mass", 2);
    addParameter(OBJECT_TYPE::I, "entropyResistance", 3);

    // Type J → ağır, yüksek direnç
    addParameter(OBJECT_TYPE::K, "mass", 5);
    addParameter(OBJECT_TYPE::K, "entropyResistance", 8);
    //////

    grid = new Cell[MAP_SIZE_X*MAP_SIZE_Y];
    objects.reserve((MAP_SIZE_X/MAP_DENSITY)*(MAP_SIZE_Y/MAP_DENSITY));
    
    int type = 0;
    int id = 0;
    for(int y = 0;y < MAP_SIZE_Y;y++) {
        for(int x = 0;x < MAP_SIZE_X;x++) {
            if(x % MAP_DENSITY != 0 || y % MAP_DENSITY != 0) continue;
            type = x % 10;
            OBJECT_TYPE choosenType = (OBJECT_TYPE) (type % 10);
            float mass = getParameter(choosenType, "mass");
            float vx = 0.1f / mass;
            float vy = 0.1f / mass;

            //objects[y*MAP_SIZE_X+x] = {id,getParameter(choosenType, "mass"),x,y,getParameter(choosenType, "entropyResistance"),vx,vy,choosenType};
            objects.emplace_back(id,getParameter(choosenType, "mass"),x,y,getParameter(choosenType, "entropyResistance"),vx,vy,choosenType);
            getCell(x, y).occupant = &objects[id];
            id++;
             
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