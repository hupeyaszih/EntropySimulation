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
    // 1. Direnç kontrolü
    if(o1->entropyResistance > 7 || o2->entropyResistance > 7)
        return false;  // çok dirençli, swap yapmaz

    // 2. Mass kontrolü
    if(o1->mass > o2->mass + 5)
        return false;  // aşırı ağır, swap edemez

    // 3. Opsiyonel: sadece aynı tipler swap edebilir
    // if(o1->type != o2->type) return false;

    return true; // diğer durumlarda swap mümkün
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
        if(totalCount == 0) return 0.f;
        float p = float(c) / totalCount;
        H -= p * std::log(p);
    }
    return H;
}

Vector2D Simulation::chooseDirectionFavoringOrder(const Object& obj) {
    std::vector<Vector2D> candidates;
    float currentEntropy = computeLocalEntropy(obj, obj.x, obj.y);
    float H_max = log(OBJECT_TYPE::COUNT-1);

    for(int y=-1; y<=1; y++){
        for(int x=-1; x<=1; x++){
            if(x==0 && y==0) continue;

            int nx = obj.x + x;
            int ny = obj.y + y;
            if(nx<0 || nx>=MAP_SIZE_X || ny<0 || ny>=MAP_SIZE_Y) continue;

            float entropy = computeLocalEntropy(obj, nx, ny);
            float deltaH = entropy - currentEntropy;

            // Bu sefer deltaH < 0 → entropiyi azaltacak yönler daha cazip
            float moveProb = std::clamp(-deltaH / H_max, 0.f, 1.f);
            moveProb *= (1.f - obj.entropyResistance/10.f) / (1.f + obj.mass/10.f);

            if((float)rand()/RAND_MAX < moveProb){
                candidates.push_back({nx, ny});
            }
        }
    }

    if(!candidates.empty()){
        return candidates[rand() % candidates.size()];
    }

    return {obj.x, obj.y}; // hareket yok
}


Vector2D Simulation::chooseDirectionFavoringEntropy(const Object& obj) {
    std::vector<Vector2D> candidates;
    float currentEntropy = computeLocalEntropy(obj, obj.x, obj.y);
    float maxEntropy = 0;
    float H_max = log(OBJECT_TYPE::COUNT-1);
    for(int y = 1;y > -2;y--) {
        for(int x = 1;x > -2;x--) {
            
             if(x==0 && y==0) continue;

            int nx = obj.x + x;
            int ny = obj.y + y;
            if(nx<0 || nx>=MAP_SIZE_X || ny<0 || ny>=MAP_SIZE_Y) continue;

            float entropy = computeLocalEntropy(obj, nx, ny);
            float deltaH = entropy - currentEntropy;
            float moveProb = std::clamp(deltaH / H_max, 0.f, 1.f);
            moveProb *= (1.f - obj.entropyResistance) / (1.f + obj.mass);

            if((float)rand()/RAND_MAX < moveProb){
                candidates.push_back({nx, ny});
            }
        }   
    }
    // candidate varsa rastgele birini seç
    if(!candidates.empty()){
        Vector2D chosen = candidates[rand()%candidates.size()];
        return chosen;
    }
    return {obj.x,obj.y};
}

void Simulation::Update(){
    for(auto& object : map->objects) {
        float H_max = log(OBJECT_TYPE::COUNT-1);
        float currentEntropy = computeLocalEntropy(object, object.x, object.y);
        float normalizedEntropy = currentEntropy/H_max;
        float orderProb = std::clamp((normalizedEntropy - ENTROPY_ORDER_THRESHOLD) / (1 - ENTROPY_ORDER_THRESHOLD), 0.f, 1.f);
        //float orderProb = std::clamp((normalizedEntropy - ENTROPY_ORDER_THRESHOLD) / (1 - ENTROPY_ORDER_THRESHOLD), 0.f, 1.f);

        orderProb *= (1.f - object.entropyResistance/10.f) / (1.f + object.mass/10.f);
        Vector2D pos = {0,0};

        if((float)rand()/RAND_MAX < orderProb){
            pos = chooseDirectionFavoringOrder(object);
        }else{
            pos = chooseDirectionFavoringEntropy(object);
        }
        int dx = pos.x - object.x;
        int dy = pos.y - object.y;
        float alpha = 0.8f; // önceki hareketin ağırlığı
        object.vx = alpha * object.vx + (1 - alpha) * dx;
        object.vy = alpha * object.vy + (1 - alpha) * dy;

        // Sonra integera çevirip move et
        int moveX = round(object.vx);
        int moveY = round(object.vy);
        move(object, round(moveX), round(moveY));
    }
}

Simulation::~Simulation() {
    delete map;
    delete this;
}