#include "core.hpp"
#include "SDL.h"
#include "SDL_error.h"
#include "SDL_events.h"
#include "SDL_keycode.h"
#include "SDL_render.h"
#include "SDL_timer.h"
#include "SDL_video.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "api.hpp"
#include "app.hpp"
#include "listener.hpp"
#include "notepad.hpp"
#include <iostream>
#include <utility>

std::pair<bool, const char*> init(){
    int img_flags=IMG_INIT_PNG;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)){
        return {false,SDL_GetError()};
    }
    if ((IMG_Init(img_flags) & img_flags) != img_flags){
        return {false,IMG_GetError()};
    }
    if (TTF_Init()){
        return {false,TTF_GetError()};
    }
    aoGetWindow() = SDL_CreateWindow(aoGetTitle(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, aoGetW(), aoGetH(), SDL_WINDOW_HIDDEN);
    if (!aoGetWindow()){
        return {false,SDL_GetError()};
    }
    aoGetRenderer() = SDL_CreateRenderer(aoGetWindow(), -1, 0);
    if (!aoGetRenderer()){
        return {false,SDL_GetError()};
    }
    return {true,"All good"};
}
void aoRun(){
    auto i = init();
    if (!i.first){
        std::cerr << i.second << "\n";
        return;
    }

    SDL_Event e;
    bool running = true;

    aoFont fo("assets/fonts/font.ttf");
    aoIcons icons;
    icons.inserticon(fo, "assets/images/notepad.png", "notepad", nullptr,aoNotepad);

    SDL_ShowWindow(aoGetWindow());
    while (running){
        while (SDL_PollEvent(&e)){
            switch (e.type) {
                case SDL_QUIT:
                    running=false;
                    break;
                case SDL_KEYUP:
                case SDL_KEYDOWN:
                    aoKReg(e.key);
                    break;
                case SDL_MOUSEMOTION:
                    aoMReg(e.motion);
                    break;
                case SDL_MOUSEWHEEL:
                    aoMReg(e.wheel);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                    aoMReg(e.button);
                    break;
                
            }
        }

        if (aoKIsPressed(SDLK_ESCAPE)){
            running = false;
        }
        if (aoKIsPressed(SDLK_LCTRL)){
            std::cout << aoNotepadGetinput();
            aoNotepadGetinput()="";
        }
        SDL_RenderClear(aoGetRenderer());
        icons.draw();
        icons.runall();
        SDL_RenderPresent(aoGetRenderer());
        
        aoKUpdate();
        aoMUpdate();
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
    TTF_Quit();
    IMG_Quit();
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