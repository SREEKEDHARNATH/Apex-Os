#pragma once

#include "SDL_render.h"
#include "api.hpp"
#include <memory>
#include <vector>
struct aoIcon{
    int x=0;
    int y=0;
    int tw=64;
    int th=10;
    int size=64;
    std::shared_ptr<SDL_Texture> ptr;
    std::shared_ptr<SDL_Texture> text;
    void(*onclick)() = nullptr;
    void(*onrun)() = nullptr;
};

class aoIcons{
    std::vector<aoIcon> icons;
    std::vector<bool> run;
    int ypadding=10,xpadding=17,textpadding=3;
    int curx=xpadding,cury=ypadding;
    public:
        void inserticon(aoFont& f, const char* imagepath, const char* txt, void(*onclick)(), void(*onrun)());
        void draw();
        void runall();
};