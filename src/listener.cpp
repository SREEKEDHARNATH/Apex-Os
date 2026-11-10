#include "listener.hpp"

#include "SDL_events.h"
#include "SDL_keyboard.h"
#include "SDL_keycode.h"
#include "SDL_mouse.h"
#include "SDL_scancode.h"

bool keys[SDL_NUM_SCANCODES] = {false};
bool prvkeys[SDL_NUM_SCANCODES] = {false};

void aoKReg(SDL_KeyboardEvent key){
    auto sc = key.keysym.scancode;
    if (sc > 0 && sc < SDL_NUM_SCANCODES){
        keys[sc]=(key.state==SDL_PRESSED);
    }
}
bool aoKIsPressed(SDL_KeyCode k){
    return aoKIsPressed(SDL_GetScancodeFromKey(k));
}
bool aoKIsPressed(SDL_Scancode sc){
    return (sc > 0 && sc < SDL_NUM_SCANCODES) ? keys[sc] && !prvkeys[sc] : false;
}
bool aoKIsDown(SDL_KeyCode k){
    return aoKIsDown(SDL_GetScancodeFromKey(k));
}
bool aoKIsDown(SDL_Scancode sc){
    return (sc > 0 && sc < SDL_NUM_SCANCODES) ? keys[sc] : false;
}
void aoKUpdate(){
    for (int i=0;i<SDL_NUM_SCANCODES;i++){
        prvkeys[i]=keys[i];
    }
}

int x=0,y=0;
float scrlx=0,scrly=0;
bool buttons[3]={false};
bool prvbuttons[3]={false};

void aoMReg(SDL_MouseMotionEvent m){
    x=m.x;
    y=m.y;
}
void aoMReg(SDL_MouseButtonEvent b){
    if (b.button > 0 && b.button < 4){
        buttons[b.button]=(b.state==SDL_PRESSED);
    }
}
void aoMReg(SDL_MouseWheelEvent w){
    scrlx=w.preciseX*(w.direction==SDL_MOUSEWHEEL_FLIPPED ? -1 : 1);
    scrly=w.preciseY*(w.direction==SDL_MOUSEWHEEL_FLIPPED ? -1 : 1);
}
bool aoMIsPressed(int b){
    return (b > 0 && b < 4) ? buttons[b] && !prvbuttons[b]: false;
}
bool aoMIsDown(int b){
    return (b > 0 && b < 4) ? buttons[b] : false;
}
int aoMGetX(){
    return x;
}
int aoMGetY(){
    return y;
}
float aoMGetScrlX(){
    return scrlx;
}
float aoMGetScrlY(){
    return scrly;
}
void aoMUpdate(){
    for (int i=0;i<3;i++){
        prvbuttons[i]=buttons[i];
    }
    scrlx=0;
    scrly=0;
}