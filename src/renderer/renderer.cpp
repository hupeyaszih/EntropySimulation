#include "renderer/renderer.h"
#include "core/map.h"
#include "core/simulation.h"
#include "globals.h"

#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_pixels.h>
#include <SDL_events.h>
#include <SDL_hints.h>
#include <SDL_keycode.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_timer.h>
#include <SDL_video.h>
#include <algorithm>
#include <cstdlib>


Renderer::Renderer(){

}

bool Renderer::Init(const Simulation& simulation) {
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

    indices.resize(6*simulation.map->objects.size());
    vertices.resize(4*simulation.map->objects.size());

    return true;
}

void Renderer::Render(const Simulation& simulation) {
    float currentFPS;
    Uint32 frameStart = SDL_GetTicks();
    float extra_time{0};
    Timer fps_cap_timer{};
    fps_cap_timer.restart();
    //Input
    
    SDL_Event event;
    
    while(SDL_PollEvent(&event)) {
        // Determine zoom direction
        float zoomStep = 1.1f;
        float oldScale = camera.zoom;
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
            case SDL_MOUSEWHEEL:
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                if (event.wheel.y > 0) camera.zoom *= zoomStep; // Zoom In
                else if (event.wheel.y < 0) camera.zoom /= zoomStep; // Zoom Out
                camera.zoom = std::clamp(camera.zoom,1.0f,10.0f);
                // Adjust offset to zoom toward mouse position
                // Formula: new_offset = mouse_pos - (mouse_pos - old_offset) * (new_scale / old_scale)
                camera.x = mouseX - (mouseX - camera.x) * (camera.zoom / oldScale);
                camera.y = mouseY - (mouseY - camera.y) * (camera.zoom / oldScale);
                break;
            default:
                break;
        }
    }
    
    if(keyboard[SDLK_w]) camera.y-=(int)camera.zoom;
    if(keyboard[SDLK_s]) camera.y+=(int)camera.zoom;
    if(keyboard[SDLK_d]) camera.x+=(int)camera.zoom;
    if(keyboard[SDLK_a]) camera.x-=(int)camera.zoom;

    if(keyboard[SDLK_SPACE]) {
        SDL_Delay(1000);
    }else {
        SDL_Delay(0);
    }
    //Render
    SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);
    SDL_RenderClear(sdl_renderer);


    
    int i = 0;
    for(auto& object : simulation.map->objects) {
        SDL_Color color;
        switch (object.type) {
            case OBJECT_TYPE::A:
                color = {0, 0, 255, 255};
                break;
            case OBJECT_TYPE::B:
                color = {0, 255, 0, 255};
                break;
            case OBJECT_TYPE::C:
                color = {255, 0, 0, 255};
                break;
            case OBJECT_TYPE::D:
                color = {255, 255, 255, 255};
                break;
        }

        SDL_FRect rect = { (float)object.x, (float)object.y, 1, 1 };
        SDL_FRect drawRect;
        drawRect.w = rect.w * camera.zoom;
        drawRect.h = rect.h * camera.zoom;
        drawRect.x = (rect.x * camera.zoom) + camera.x;
        drawRect.y = (rect.y * camera.zoom) + camera.y;

        int v = i * 4;
        vertices[v + 0] = (SDL_Vertex){{drawRect.x, drawRect.y},     color, {0,0}}; // Sol Üst
        vertices[v + 1] = (SDL_Vertex){{drawRect.x+drawRect.w, drawRect.y},   color, {0,0}}; // Sağ Üst
        vertices[v + 2] = (SDL_Vertex){{drawRect.x+drawRect.w, drawRect.y+drawRect.h}, color, {0,0}}; // Sağ Alt
        vertices[v + 3] = (SDL_Vertex){{drawRect.x, drawRect.y+drawRect.h},   color, {0,0}}; // Sol Alt

        int ind = i * 6; 
        indices[ind + 0] = v + 0;
        indices[ind + 1] = v + 1;
        indices[ind + 2] = v + 2;
        indices[ind + 3] = v + 0;
        indices[ind + 4] = v + 2;
        indices[ind + 5] = v + 3;

        i++;
    }
    //SDL_RenderFillRectsF(sdl_renderer,rects,i);
    SDL_RenderGeometry(sdl_renderer, NULL, vertices.data(), vertices.size(), indices.data(), indices.size());
    //Present Scene
    SDL_RenderPresent(sdl_renderer);

    //DELAY
    //SDL_Delay(100);
   

    while((fps_cap_timer.get_time() + extra_time) < SCREEN_TICKS_PER_FRAME) {
        SDL_Delay(1); // I am aware of the issues with delay on different platforms / OS scheduling
    }
    Uint32 frameTime = SDL_GetTicks() - frameStart;
    if (fps_cap_timer.get_time() < (SCREEN_TICKS_PER_FRAME)) {
        extra_time -= SCREEN_TICKS_PER_FRAME - fps_cap_timer.get_time();
    }else {
        extra_time += fps_cap_timer.get_time() - SCREEN_TICKS_PER_FRAME;
    }


    currentFPS = 1000.0f / frameTime;
    LOG(currentFPS);

}