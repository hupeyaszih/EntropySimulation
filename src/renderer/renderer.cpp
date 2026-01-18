#include "renderer/renderer.h"
#include "core/map.h"
#include "core/simulation.h"
#include "globals.h"

#include <SDL_events.h>
#include <SDL_hints.h>
#include <SDL_keycode.h>
#include <SDL_render.h>
#include <SDL_timer.h>
#include <SDL_video.h>
#include <cstdlib>


Renderer::Renderer(){

}

bool Renderer::Init() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        LOG("SDL COULDN'T INITIALIZE:");
        return false;
    }

    sdl_window = SDL_CreateWindow("MATRIX", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if(!sdl_window) {
        LOG("Failed to open sdl_window");
        return false;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    
    sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);

    if(!sdl_renderer) {
        LOG("Failed to create sdl_renderer");
    }

    LOG("Renderer Initialized");
    return true;
}

void Renderer::Render(Simulation& simulation) {

    //Input
    SDL_Event event;
    
    while(SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                exit(0);
                break;
            case SDL_KEYDOWN:
                keyboard[event.key.keysym.sym] = false;
                break;
            case SDL_KEYUP:
                keyboard[event.key.keysym.sym] = true;
                break;
            default:
                break;
        }
    }

    if(keyboard[SDLK_w]) camera.y++;
    if(keyboard[SDLK_s]) camera.y--;
    if(keyboard[SDLK_d]) camera.x++;
    if(keyboard[SDLK_a]) camera.x--;

    //Render
    SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);
    SDL_RenderClear(sdl_renderer);


    for(auto& object : simulation.map.objects) {
        switch (object.type) {
            case OBJECT_TYPE::A:
                SDL_SetRenderDrawColor(sdl_renderer, 255, 255, 255, 255);
                break;
            case OBJECT_TYPE::B:
                SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 255, 255);
                break;
            case OBJECT_TYPE::C:
                SDL_SetRenderDrawColor(sdl_renderer, 0, 255, 0, 255);
                break;
            case OBJECT_TYPE::D:
                SDL_SetRenderDrawColor(sdl_renderer, 255, 0, 0, 255);
                break;
        }

        SDL_RenderDrawPoint(sdl_renderer,object.x+camera.x,object.y-camera.y);
    }

    //Present Scene
    SDL_RenderPresent(sdl_renderer);

    //DELAY
    SDL_Delay(100);
}