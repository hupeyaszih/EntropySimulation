#include "core/simulation.h"

#include "core/map.h"
#include "globals.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <random>

Simulation::Simulation() {

}

void Simulation::Init() {
    map = new Map();
    map->Init();
    LOG("Simulation Initialized..");
}

void Simulation::move(Object& object, int dirX,int dirY){
    int newX = object.x + dirX;
    int newY = object.y + dirY;
    if(newX < 0 || newX >= MAP_SIZE_X) return;
    if(newY < 0 || newY >= MAP_SIZE_Y) return;

    Cell& oldCell = map->getCell(object.x, object.y);
    Cell& newCell = map->getCell(newX, newY);
    if(newCell.occupant != nullptr) {
        Object* target = newCell.occupant;
        if(!canSwap(&object, target)) return;
                
        newCell.occupant = &object;
        oldCell.occupant = target;

        std::swap(object.x, target->x);
        std::swap(object.y, target->y);
        
        return;
    }
    map->getCell(object.x, object.y).occupant = nullptr;
    object.x = newX;
    object.y = newY;
    map->getCell(object.x, object.y).occupant = &object;
}

bool Simulation::canSwap(const Object* o1, const Object* o2) const {
    //
    return true;
}

float Simulation::computeLocalEntropy(const Object& obj,int posX,int posY) {
    int totalNeighbour = 8;
    int TOTAL_TYPE = OBJECT_TYPE::COUNT;
    std::vector<int> counts(TOTAL_TYPE, 0);

    for(int y = 1;y > -2;y--) {
        for(int x = 1;x > -2;x--) {
            if(x == 0 && y == 0) continue;
            int targetX = posX + x;
            int targetY = posY + y;
            if(targetX < 0 || targetX >= MAP_SIZE_X) continue;
            if(targetY < 0 || targetY >= MAP_SIZE_Y) continue;
            Cell& cell = map->getCell(targetX, targetY);

            if(cell.occupant == nullptr) continue;
            int type = cell.occupant->type;
            counts[type]++;
        }   
    }

    int totalCount = 0;
    for(int c : counts) totalCount += c;

    float H = 0.f;
    for(int c : counts){
        if(c == 0) continue;
        float p = float(c) / totalCount;
        H -= p * std::log(p);
    }
    return H;
}

Vector2D Simulation::chooseDirection(const Object& obj) {
    float currentEntropy = computeLocalEntropy(obj, obj.x, obj.y);
    Vector2D bestPOS = {obj.x,obj.y};
    float maxEntropy = 0;

    for(int y = 1;y > -2;y--) {
        for(int x = 1;x > -2;x--) {
            float entropy = computeLocalEntropy(obj, obj.x+x, obj.y+y);
            float deltaEntropy = entropy-currentEntropy;
            float moveProb = 0;
            float H_max = log(OBJECT_TYPE::COUNT-1);
            moveProb = std::clamp(deltaEntropy / H_max, 0.f, 1.f);
            moveProb *= (1.f - obj.entropyResistance) / (1.f + obj.mass);
            if(entropy > maxEntropy) {
                if(rand()%1 < moveProb) {
                    maxEntropy = entropy;
                bestPOS = {obj.x+x,obj.y+y};
                }
                
            }
        }   
    }
    return bestPOS;
}

void Simulation::Update(){
    for(auto& object : map->objects) {
        Vector2D pos = chooseDirection(object);
        int dx = pos.x - object.x;
        int dy = pos.y - object.y;
        move(object, dx, dy);
    }
}

Simulation::~Simulation() {
    delete map;
    delete this;
}