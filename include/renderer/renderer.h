#ifndef RENDERER_H
#define RENDERER_H

#include "core/simulation.h"
#include <map>
#define WINDOW_WIDTH 650
#define WINDOW_HEIGHT 500


#include "SDL2/SDL.h"

struct Camera{
    int x = 0;
    int y = 0;
    int zoom;
};

class Renderer{
private:
    SDL_Window* sdl_window = NULL;
    SDL_Renderer* sdl_renderer = NULL;
    Camera camera;
    std::map<int, bool> keyboard; // Saves the state(true=pressed; false=released) of each SDL_Key.
   
public:
    Renderer();
    bool Init();
    void Render(Simulation& simulation);
};

#endif