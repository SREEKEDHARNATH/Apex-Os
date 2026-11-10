#pragma once

#include "SDL_render.h"
#include "SDL_ttf.h"
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

class aoFont{
    std::shared_ptr<TTF_Font> ptr;
    public:
        aoFont(const char* name, int ptsize=32,bool load=true);
        aoFont(TTF_Font* f);
        void set(const char* name, int ptsize=32);
        void set(TTF_Font* f);
        TTF_Font* get();
};