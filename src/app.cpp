#include "app.hpp"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "api.hpp"
#include "core.hpp"
#include "listener.hpp"

void aoIcons::inserticon(aoFont& f, const char* imagepath, const char* txt, void(*onclick)()){
    aoIcon ic;
    if ((curx+ic.size+xpadding) > aoGetW()){
        return;
    }
    aoImage im(imagepath);
    aoText t(f,txt);
    ic.x=curx;
    ic.y=cury;
    ic.ptr=im.get();
    ic.onclick=onclick;
    ic.text=t.image.get();
    ic.tw=t.image.r.w;
    ic.th=t.image.r.h;
    icons.push_back(ic);

    cury+=ic.size+ypadding+ic.th;
    if ((cury+ic.size+ypadding) > aoGetH()){
        cury=ypadding;
        curx+=xpadding+ic.size;
    }
}
void aoIcons::draw(){
    SDL_Rect mouse = {aoMGetX(),aoMGetY(),1,1,};
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
                    if (ic.onclick){
                        ic.onclick();
                    }
                }
            }
        }
    }
}