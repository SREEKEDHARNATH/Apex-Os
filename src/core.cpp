#include "core.hpp"
#include "SDL.h"
#include "SDL_error.h"
#include "SDL_events.h"
#include "SDL_render.h"
#include "SDL_timer.h"
#include "SDL_video.h"
#include <iostream>
#include <string>

const char* init(){
    if (SDL_Init(SDL_INIT_EVERYTHING)){
        return SDL_GetError();
    }
    aoGetWindow() = SDL_CreateWindow(aoGetTitle(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, aoGetW(), aoGetH(), SDL_WINDOW_HIDDEN);
    if (aoGetWindow()){
        return SDL_GetError();
    }
    aoGetRenderer() = SDL_CreateRenderer(aoGetWindow(), -1, 0);
    if (aoGetRenderer()){
        return SDL_GetError();
    }
    return "";
}
void aoRun(){
    auto i = init();
    if (!std::string(i).empty()){
        std::cerr << i << "\n";
        return;
    }

    SDL_Event e;
    bool running = true;

    SDL_ShowWindow(aoGetWindow());
    while (running){
        while (SDL_PollEvent(&e)){
            switch (e.type) {
                case SDL_QUIT:
                    running=false;
                    break;
            }
        }

        SDL_RenderClear(aoGetRenderer());
        SDL_RenderPresent(aoGetRenderer());
        
        SDL_Delay(16);
    }
}
void aoQuit(){
    if (aoGetRenderer()){
        SDL_DestroyRenderer(aoGetRenderer());
    }
    if (aoGetWindow()){
        SDL_DestroyWindow(aoGetWindow());
    }
    SDL_Quit();
}

int aoGetW(){
    return 1280;
}
int aoGetH(){
    return 720;
}
const char* aoGetTitle(){
    return "Apex Os";
}

SDL_Window*& aoGetWindow(){
    static SDL_Window* window = nullptr;
    return window;
}
SDL_Renderer*& aoGetRenderer(){
    static SDL_Renderer* renderer = nullptr;
    return renderer;
}