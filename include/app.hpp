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
};

struct aoIcons{
    std::vector<aoIcon> icons;
    void inserticon(aoFont& f, const char* imagepath, const char* txt, void(*onclick)());
    void draw();
    private:
        int ypadding=10,xpadding=17,textpadding=3;
        int curx=xpadding,cury=ypadding;
};