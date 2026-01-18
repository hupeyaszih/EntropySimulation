#include "core/map.h"
#include "globals.h"
Map::Map() {

}

void Map::Init() {
    objects.reserve(MAP_SIZE_X*MAP_SIZE_Y);
    int type = 0;
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

            objects.emplace_back(y*MAP_SIZE_X+x,1,x,y,1,choosenType);
            type++;
        }   
    }
}