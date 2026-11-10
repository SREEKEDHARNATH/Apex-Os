#pragma once

#include "SDL_render.h"
#include "api.hpp"
#include <memory>
#include <vector>

struct aoWindow{
    int x=200,y=50,w=800,h=600;
    std::shared_ptr<SDL_Texture> bg;
    std::shared_ptr<SDL_Texture> close;
    std::shared_ptr<SDL_Texture> uppertile;
};
struct aoIcon{
    int x=0;
    int y=0;
    int tw=64;
    int th=10;
    int size=64;
    std::shared_ptr<SDL_Texture> ptr;
    std::shared_ptr<SDL_Texture> text;
    void(*onclick)() = nullptr;
    bool(*onrun)(aoIcon&) = nullptr;
    aoWindow window;
};

class aoIcons{
    std::vector<aoIcon> icons;
    std::vector<bool> run;
    int ypadding=10,xpadding=17,textpadding=3;
    int curx=xpadding,cury=ypadding;
    public:
        void inserticon(aoFont& f, const char* imagepath, const char* txt, void(*onclick)(), bool(*onrun)(aoIcon&));
        void draw();
        void runall();
};
