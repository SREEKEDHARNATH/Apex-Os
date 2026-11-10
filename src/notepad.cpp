#include "notepad.hpp"
#include "SDL_keycode.h"
#include "SDL_scancode.h"
#include "listener.hpp"

void aoNotepad(){
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
}
std::string& aoNotepadGetinput(){
    static std::string input;
    return input;
}