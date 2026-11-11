#include "core.hpp"
#include "SDL.h"
#include "SDL_error.h"
#include "SDL_events.h"
#include "SDL_keycode.h"
#include "SDL_mouse.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_timer.h"
#include "SDL_video.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "api.hpp"
#include "app.hpp"
#include "listener.hpp"
#include "notepad.hpp"
#include <chrono>
#include <fstream>
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
struct time{
    int hours=0;
    int minutes=0;
    int seconds=0;
    bool am=true;
};
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
    aoImage shutdown("assets/images/shutdown.png");
    shutdown.r.x=aoGetW()-shutdown.r.w;

    struct time t;
    
    auto cur = std::chrono::system_clock::now();
    auto local = std::chrono::current_zone();
    auto ti = local->to_local(cur);
    auto s = std::chrono::time_point_cast<std::chrono::seconds>(ti).time_since_epoch().count()+6;
    while (s){
        s--;
        t.seconds++;
        if (t.seconds==60){
            t.seconds=0;
            t.minutes++;
        }
        if (t.minutes==60){
            t.minutes=0;
            t.hours++;
        }
        if (t.hours==12){
            t.hours=0;
            t.am = !t.am;
        }
    }

    aoText timetext(fo, "00::00::00 AM");
    std::ifstream cfg("config.txt");
    if (cfg.is_open()){
        std::string line;
        for (int i=0;i<=icons.icons.size();i++){
            for (int j=0;j<2;j++){
                std::getline(cfg, line);
                if (line.empty()){
                    continue;
                }
                if (i){
                    if (j){
                        icons.icons[i-1].y=std::stoi(line);
                    }
                    else {
                        icons.icons[i-1].x=std::stoi(line);
                    }
                }
                else {
                    if (j){
                        timetext.image.r.y=std::stoi(line);
                    }
                    else {
                        timetext.image.r.x=std::stoi(line);
                    }
                }
            }   
        }
        cfg.close();
    }
    auto start=std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
    bool hold=false;
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
        SDL_Rect mouse = {aoMGetX(),aoMGetY(),1,1};
        if (aoKIsPressed(SDLK_ESCAPE)){
            running = false;
        }

        if (SDL_HasIntersection(&shutdown.r, &mouse)){
            if (aoMIsPressed(SDL_BUTTON_LEFT)){
                running=false;
            }
        }

        if (aoKIsDown(SDLK_LCTRL) && aoKIsDown(SDLK_LALT) && aoKIsDown(SDLK_LSHIFT) && aoKIsPressed(SDLK_s)){
            running=false;
        }

        if (aoKIsPressed(SDLK_RIGHT)){
            timetext.image.r.x+=64;
        }
        if (aoKIsPressed(SDLK_LEFT)){
            timetext.image.r.x-=64;
        }
        if (aoKIsPressed(SDLK_UP)){
            timetext.image.r.y-=64;
        }
        if (aoKIsPressed(SDLK_DOWN)){
            timetext.image.r.y+=64;
        }
        
        if (SDL_HasIntersection(&timetext.image.r, &mouse) || hold){
            if (aoMIsDown(SDL_BUTTON_LEFT)){
                hold=true;
                timetext.image.r.x = ((mouse.x)/64)*64;
                timetext.image.r.y = ((mouse.y)/64)*64;
            }
            else {
                hold=false;
            }
        }

        while ((timetext.image.r.x+timetext.image.r.w) > aoGetW()){
            timetext.image.r.x-=64;
        }
        while (timetext.image.r.x < 0){
            timetext.image.r.x+=64;
        }
        while (timetext.image.r.y < 0){
            timetext.image.r.y+=64;
        }
        while ((timetext.image.r.y+timetext.image.r.h) > aoGetH()){
            timetext.image.r.y-=64;
        }

        if ((std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::steady_clock().now()).time_since_epoch().count()-start)==1){
            start=std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
            auto st = std::string((std::to_string(t.hours).size() == 1 ? std::string("0" + std::to_string(t.hours)) : std::to_string(t.hours))
            + "::" + (std::to_string(t.minutes).size() == 1 ? std::string("0" + std::to_string(t.minutes)) : std::to_string(t.minutes))
            + "::" + (std::to_string(t.seconds).size() == 1 ? std::string("0" + std::to_string(t.seconds)) : std::to_string(t.seconds))
            + " " + (t.am ? "AM" : "PM"));
            timetext.set(fo, st.c_str());
            t.seconds++;
        }
        if (t.seconds==60){
            t.seconds=0;
            t.minutes++;
        }
        if (t.minutes==60){
            t.minutes=0;
            t.hours++;
        }
        if (t.hours==12){
            t.hours=0;
            t.am = !t.am;
        }

        SDL_RenderClear(aoGetRenderer());
        shutdown.draw();
        timetext.draw();
        icons.draw();
        icons.runall();
        SDL_RenderPresent(aoGetRenderer());
        
        aoKUpdate();
        aoMUpdate();

        SDL_Delay(16);
    }
    std::ofstream config("config.txt", std::ios::trunc);
    if (config.is_open()){
        config << timetext.image.r.x << "\n" << timetext.image.r.y << "\n";
        for (auto& a : icons.icons){
            config << a.x << "\n" << a.y << "\n";
        }
        config.close();
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