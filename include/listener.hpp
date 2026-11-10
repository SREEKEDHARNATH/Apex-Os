#pragma once

#include "SDL_events.h"
#include "SDL_keycode.h"

void aoKReg(SDL_KeyboardEvent key);
bool aoKIsPressed(SDL_KeyCode k);
bool aoKIsPressed(SDL_Scancode sc);
bool aoKIsDown(SDL_KeyCode k);
bool aoKIsDown(SDL_Scancode sc);
void aoKUpdate();

void aoMReg(SDL_MouseMotionEvent m);
void aoMReg(SDL_MouseButtonEvent b);
void aoMReg(SDL_MouseWheelEvent w);
bool aoMIsPressed(int b);
bool aoMIsDown(int b);
int aoMGetX();
int aoMGetY();
float aoMGetScrlX();
float aoMGetScrlY();
void aoMUpdate();