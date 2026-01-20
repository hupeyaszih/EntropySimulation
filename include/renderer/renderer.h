#ifndef RENDERER_H
#define RENDERER_H

#include "core/simulation.h"
#include <map>
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500


#include <SDL2/SDL.h>

struct Camera{
    int x = 0;
    int y = 0;
    float zoom = 1;
};

const float TARGET_FPS = 60.04f;
const float SCREEN_TICKS_PER_FRAME = 1000.0f / static_cast<float>(TARGET_FPS);
class Timer{
public:
    Uint64 started_ticks = 0;

    Timer()
            :started_ticks{SDL_GetPerformanceCounter()}
    {}

    void restart() {
        started_ticks = SDL_GetPerformanceCounter();
    }

    float get_time() {
        return (static_cast<float>(SDL_GetPerformanceCounter() - started_ticks) / static_cast<float>(SDL_GetPerformanceFrequency()) * 1000.0f);
    }
};

class Renderer{
private:
    SDL_Window* sdl_window = NULL;
    SDL_Renderer* sdl_renderer = NULL;
    Camera camera;
    std::map<int, bool> keyboard;
   
    std::vector<int> indices;
    std::vector<SDL_Vertex> vertices;

public:
    Renderer();
    bool Init(const Simulation& simulation);
    void Render(const Simulation& simulation);
};

#endif
