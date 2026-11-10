#include "api.hpp"
#include "SDL_error.h"
#include "SDL_image.h"
#include "SDL_pixels.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include "core.hpp"
#include <iostream>

aoImage::aoImage(const char* name, bool load, bool autoset){
    if (load){
        set(name,autoset);
    }
}
aoImage::aoImage(SDL_Texture* t,bool autoset){
    set(t,autoset);
}
aoImage::aoImage(Uint8 r,Uint8 g,Uint8 b,int w,int h,bool autoset){
    set(r,g,b,w,h,autoset);
}
void aoImage::set(const char* name, bool autoset){
    auto t = IMG_LoadTexture(aoGetRenderer(), name);
    if (!t){
        std::cerr << IMG_GetError() << "\n";
        return;
    }
    ptr.reset(t, SDL_DestroyTexture);
    if (autoset){
        SDL_QueryTexture(t, NULL,NULL, &r.w,&r.h);
    }
}
void aoImage::set(SDL_Texture *t, bool autoset){
    if (t){
        ptr.reset(t, SDL_DestroyTexture);
        if (autoset){
            SDL_QueryTexture(t, NULL,NULL, &r.w,&r.h);
        }
    }
}
void aoImage::set(Uint8 r, Uint8 g, Uint8 b,int w,int h,bool autoset){
    if (w < 1){
        w=32;
    }
    if (h < 1){
        h=32;
    }
    auto surf = SDL_CreateRGBSurface(0,w,h,32,0,0,0,0);
    if (!surf){
        std::cerr << SDL_GetError() << "\n";
        return;
    }
    auto colour = SDL_MapRGB(surf->format, r, g, b);
    SDL_FillRect(surf, NULL, colour);
    auto t = SDL_CreateTextureFromSurface(aoGetRenderer(), surf);
    if (!t){
        std::cerr << SDL_GetError() << "\n";
        return;
    }
    set(t,autoset);
}
void aoImage::draw(){
    if (get()){
        if (r.w<=0 || r.h<=0){
            SDL_RenderCopy(aoGetRenderer(), get(), NULL,NULL);
        }
        else {
            SDL_RenderCopy(aoGetRenderer(), get(), NULL,&r);
        }
    }
}
SDL_Texture* aoImage::get(){
    return ptr.get();
}