#include "notepad.hpp"
#include "SDL_keycode.h"
#include "SDL_mouse.h"
#include "SDL_pixels.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_scancode.h"
#include "api.hpp"
#include "app.hpp"
#include "core.hpp"
#include "listener.hpp"
#include <fstream>
#include <vector>

bool holding=false;
SDL_Rect holdedMousePos;
std::vector<SDL_Rect> contents;
std::vector<aoText> texts;

void saveinput(){
    std::ofstream save("save.txt", std::ios::trunc);
    if (save.is_open()){
        for (auto& a : texts){
            save << a.getText() << "\n";
        }
        save.close();
    }
}
void handlekeys();
bool aoNotepad(aoIcon& icon){
    auto& window = icon.window;
    auto ptr = icon.font;
    if (!(window.bg.get() && window.uppertile.get() && window.close.get() && icon.text.get() && ptr)){
        holding=false;
        return false;
    }
    if (aoKIsDown(SDLK_LCTRL) && aoKIsPressed(SDLK_s)){
        saveinput();
        return true;
    }
    handlekeys();
    
    static aoImage file("assets/images/file-notouch.png");
    static aoImage filetouch("assets/images/file-touch.png");
    static aoImage save("assets/images/save-notouch.png");
    static aoImage savetouch("assets/images/save-touch.png");

    static int cury=window.y+56;
    static int curx=window.x;
    SDL_Rect mouse = {aoMGetX(),aoMGetY(),1,1};

    SDL_Rect win={window.x, window.y, window.w, window.h};
    SDL_Rect uppertile={window.x, window.y, window.w, 30};
    SDL_Rect close={(win.x+win.w)-25, window.y+5, 20, 20};
    SDL_Rect text={window.x, window.y, 0,0};
    
    SDL_Rect fileR={window.x, window.y+30, 256, 32};
    SDL_Rect saveR={window.x+256, window.y+30, 256, 32};
    
    if (aoKIsPressed(SDLK_RETURN) || aoKIsPressed(SDLK_RETURN2)){
        bool enter=false;
        if (!aoNotepadGetinput().empty()){
            aoNotepadGetinput() = aoNotepadGetinput().substr(0,aoNotepadGetinput().size()-1);
            texts.emplace_back(*ptr, aoNotepadGetinput().c_str(), (SDL_Color){255,0,0,255});
            cury+=30;
            SDL_Rect content={curx, cury, window.w, window.h};
            SDL_QueryTexture(texts[texts.size()-1].image.get().get(), NULL,NULL,&content.w,&content.h);
            contents.push_back(content);
            aoNotepadGetinput()="";
        }
    }
    
    text.w/=2;
    text.h/=2;
    text.x+=((win.w-text.w)/2.0);
    SDL_QueryTexture(icon.text.get(), NULL,NULL,&text.w,&text.h);
    if (SDL_HasIntersection(&mouse, &close) && aoMIsPressed(SDL_BUTTON_LEFT)){
        holding=false;
        contents.clear();
        texts.clear();
        cury=window.y+56;
        curx=window.x;
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
        bool move = true;
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
        else {
            move=false;
        }
        if (move){
            curx=window.x;
            cury=window.y+56;
            for (auto& c : contents){
                c.x=curx;
                savetouch.r=saveR;
                c.y=cury;
                cury+=30;
            }
        }
    }
    
    SDL_RenderCopy(aoGetRenderer(), window.bg.get(), NULL, &win);
    SDL_RenderCopy(aoGetRenderer(), window.uppertile.get(), NULL, &uppertile);
    SDL_RenderCopy(aoGetRenderer(), window.close.get(), NULL, &close);
    SDL_RenderCopy(aoGetRenderer(), icon.text.get(), NULL, &text);
    file.r=fileR;
    filetouch.r=fileR;
    save.r=saveR;
    savetouch.r=saveR;
    for (int i=0;i<texts.size();i++){
        texts[i].image.r=contents[i];
        texts[i].draw();
    }
    static bool touchedfile=false;
    if (SDL_HasIntersection(&fileR, &mouse)){
        touchedfile=true;
        filetouch.draw();
    }
    else {
        file.draw();
        if (!SDL_HasIntersection(&saveR, &mouse)){
            touchedfile=false;
        }
    }
    if (touchedfile){
        if (SDL_HasIntersection(&saveR, &mouse)){
            savetouch.draw();
            if (aoMIsPressed(SDL_BUTTON_LEFT)){
                saveinput();
                touchedfile=false;
            }
        }
        else {
            save.draw();
        }
    }
    return true;
}
std::string& aoNotepadGetinput(){
    static std::string input;
    return input;
}
void handlekeys(){
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
        aoNotepadGetinput()+= "        ";
    }
    if (aoKIsPressed(SDLK_BACKSPACE)){
        if (aoNotepadGetinput().size()>1){
            aoNotepadGetinput() = aoNotepadGetinput().substr(0,aoNotepadGetinput().size()-1);
        }
        else {
            aoNotepadGetinput() = "";
        }
    }
}