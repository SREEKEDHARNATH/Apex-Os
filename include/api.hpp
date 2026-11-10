#pragma once

#include "SDL_render.h"
#include <memory>
class aoImage{
    std::shared_ptr<SDL_Texture> ptr;
    public:
        SDL_Rect r={0,0,0,0};
        aoImage(const char* name, bool load=true,bool autoset=true);
        aoImage(SDL_Texture* t,bool autoset=true);
        aoImage(Uint8 r,Uint8 g,Uint8 b,int w=32,int h=32,bool autoset=true);
        void set(const char* name,bool autoset=true);
        void set(Uint8 r,Uint8 g,Uint8 b,int w=32,int h=32,bool autoset=true);
        void set(SDL_Texture* t,bool autoset=true);
        void draw();
        SDL_Texture* get();
};