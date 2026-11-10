#include "notepad.hpp"
#include "SDL_keycode.h"
#include "SDL_mouse.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_scancode.h"
#include "app.hpp"
#include "core.hpp"
#include "listener.hpp"

bool holding=false;
SDL_Rect holdedMousePos;

bool aoNotepad(aoIcon& icon){
    bool shift=aoKIsDown(SDLK_LSHIFT) || aoKIsDown(SDLK_RSHIFT);
    for (int i=4;i<40;i++){
        if (aoKIsPressed((SDL_Scancode)i)){
            if (i < 30){
                aoNotepadGetinput()+= (char)(i+(shift ? 61 : 93));
            }
            else {
                if (shift){
                    char c;
                    if (i==30){
                        c='!';
                    }
                    else if (i==31){
                        c='@';
                    }
                    else if (i==32){
                        c='#';
                    }
                    else if (i==33){
                        c='$';
                    }
                    else if (i==34){
                        c='%';
                    }
                    else if (i==35){
                        c='^';
                    }
                    else if (i==36){
                        c='&';
                    }
                    else if (i==37){
                        c='*';
                    }
                    else if (i==38){
                        c='(';
                    }
                    else {
                        c=')';
                    }
                    aoNotepadGetinput()+= c;
                }
                else {
                    if (i == 39){
                        aoNotepadGetinput()+= '0';
                    }
                    else {
                        aoNotepadGetinput()+= (char)(i+19);
                    }
                }
            }
        }
    }
    char c=' ';
    if (aoKIsPressed(SDL_SCANCODE_MINUS)){
        if (shift){
            c='_';
        }
        else {
            c='-';
        }
    }
    if (aoKIsPressed(SDL_SCANCODE_EQUALS)){
        if (shift){
            c='+';
        }
        else {
            c='=';
        }
    }
    if (aoKIsPressed(SDL_SCANCODE_LEFTBRACKET)){
        if (shift){
            c='{';
        }
        else {
            c='[';
        }
    }
    if (aoKIsPressed(SDL_SCANCODE_RIGHTBRACKET)){
        if (shift){
            c='}';
        }
        else {
            c=']';
        }
    }
    if (aoKIsPressed(SDL_SCANCODE_BACKSLASH)){
        if (shift){
            c='|';
        }
        else {
            c='\\';
        }
    }
    if (aoKIsPressed(SDL_SCANCODE_SEMICOLON)){
        if (shift){
            c=':';
        }
        else {
            c=';';
        }
    }
    if (aoKIsPressed(SDL_SCANCODE_APOSTROPHE)){
        if (shift){
            c='"';
        }
        else {
            c='\'';
        }
    }
    if (aoKIsPressed(SDL_SCANCODE_GRAVE)){
        if (shift){
            c='~';
        }
        else {
            c='`';
        }
    }
    if (aoKIsPressed(SDL_SCANCODE_COMMA)){
        if (shift){
            c='<';
        }
        else {
            c=',';
        }
    }
    if (aoKIsPressed(SDL_SCANCODE_PERIOD)){
        if (shift){
            c='>';
        }
        else {
            c='.';
        }
    }
    if (aoKIsPressed(SDL_SCANCODE_SLASH)){
        if (shift){
            c='?';
        }
        else {
            c='/';
        }
    }
    if (c != ' '){
        aoNotepadGetinput()+= c;
    }
    if (aoKIsPressed(SDLK_RETURN) || aoKIsPressed(SDLK_RETURN2)){
        aoNotepadGetinput()+= "\n";
    }
    if (aoKIsPressed(SDLK_SPACE)){
        aoNotepadGetinput()+= " ";
    }
    if (aoKIsPressed(SDLK_TAB)){
        aoNotepadGetinput()+= "    ";
    }
    auto& window = icon.window;
    if (!(window.bg.get() && window.uppertile.get() && window.close.get())){
        holding=false;
        return false;
    }

    SDL_Rect mouse = {aoMGetX(),aoMGetY(),1,1};

    SDL_Rect win={window.x, window.y, window.w, window.h};
    SDL_Rect uppertile={window.x, window.y, window.w, 30};
    SDL_Rect close={(win.x+win.w)-25, window.y+5, 20, 20};

    if (SDL_HasIntersection(&mouse, &close) && aoMIsPressed(SDL_BUTTON_LEFT)){
        holding=false;
        return false;
    }
    if (SDL_HasIntersection(&mouse, &uppertile) && aoMIsDown(SDL_BUTTON_LEFT)){
        if (!holding){
            holdedMousePos=mouse;
        }
        holding=true;
    }
    else {
        holding=false;
    }
    if (holding){
        if (holdedMousePos.x > mouse.x){
            window.x-=(holdedMousePos.x-mouse.x);
            holdedMousePos.x=mouse.x;
        }
        else if (mouse.x > holdedMousePos.x){
            window.x+=(mouse.x-holdedMousePos.x);
            holdedMousePos.x=mouse.x;
        }

        if (holdedMousePos.y > mouse.y){
            window.y-=(holdedMousePos.y-mouse.y);
            holdedMousePos.y=mouse.y;
        }
        else if (mouse.y > holdedMousePos.y){
            window.y+=(mouse.y-holdedMousePos.y);
            holdedMousePos.y=mouse.y;
        }
    }

    SDL_RenderCopy(aoGetRenderer(), window.bg.get(), NULL, &win);
    SDL_RenderCopy(aoGetRenderer(), window.uppertile.get(), NULL, &uppertile);
    SDL_RenderCopy(aoGetRenderer(), window.close.get(), NULL, &close);

    return true;
}
std::string& aoNotepadGetinput(){
    static std::string input;
    return input;
}