#include "app.hpp"
#include "SDL_mouse.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "api.hpp"
#include "core.hpp"
#include "listener.hpp"
#include <vector>

void aoIcons::inserticon(aoFont& f, const char* imagepath, const char* txt, void(*onclick)(), bool(*onrun)(aoIcon&)){
    aoIcon ic;
    if ((curx+ic.size+xpadding) > aoGetW()){
        return;
    }
    aoImage im(imagepath);
    aoText t(f,txt);
    aoImage bg(255,255,255);
    aoImage uppertile((Uint8)20,20,20);
    aoImage close("assets/images/close.png");
    ic.x=curx;
    ic.y=cury;
    ic.ptr=im.get();
    ic.onclick=onclick;
    ic.text=t.image.get();
    ic.tw=t.image.r.w;
    ic.th=t.image.r.h;
    ic.onrun=onrun;
    ic.window.bg=bg.get();
    ic.window.uppertile=uppertile.get();
    ic.window.close=close.get();
    ic.font=&f;
    icons.push_back(ic);

    cury+=ic.size+ypadding+ic.th;
    if ((cury+ic.size+ypadding) > aoGetH()){
        cury=ypadding;
        curx+=xpadding+ic.size;
    }

    run.push_back(false);
}
void aoIcons::draw(){
    SDL_Rect mouse = {aoMGetX(),aoMGetY(),1,1,};
    int count=0;
    for (const auto& ic : icons){
        if (ic.ptr.get()){
            SDL_Rect r={ic.x, ic.y, ic.size,ic.size};
            SDL_RenderCopy(aoGetRenderer(), ic.ptr.get(), NULL, &r);
            if (ic.text.get()){
                auto rt = r;
                rt.w=ic.tw;
                rt.h=ic.th;
                rt.y=r.y+r.h+textpadding;
                if (rt.w > r.w){
                    rt.h = (rt.h/(float)rt.w)*r.w;
                    rt.w=r.w;
                }
                SDL_RenderCopy(aoGetRenderer(), ic.text.get(), NULL, &rt);
            }
            if (SDL_HasIntersection(&r, &mouse)){
                if (aoMIsPressed(SDL_BUTTON_LEFT)){
                    run[count]=true;
                    if (ic.onclick){
                        ic.onclick();
                    }
                }
            }
        }
        count++;
    }
}
std::vector<bool> holds;
void aoIcons::runall(){
    while (holds.size() < icons.size()){
        holds.push_back(false);
    }
    for (int i=0;i<icons.size();i++){
        if (run[i]){
            SDL_Rect iconrect = {icons[i].x, icons[i].y, icons[i].size, icons[i].size};
            SDL_Rect mouse = {aoMGetX(),aoMGetY(),1,1};
            if (SDL_HasIntersection(&iconrect, &mouse) || holds[i]){
                if (aoMIsDown(SDL_BUTTON_LEFT)){
                    holds[i] = true;
                    icons[i].x = (((int)mouse.x)/(int)icons[i].size)*icons[i].size;
                    icons[i].y = (((int)mouse.y)/(int)icons[i].size)*icons[i].size;
                    continue;
                }
                else {
                    holds[i]=false;
                }
            }
            if (icons[i].onrun){
                run[i] = icons[i].onrun(icons[i]);
            }
        }
    }
}