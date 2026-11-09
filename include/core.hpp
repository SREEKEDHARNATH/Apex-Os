#pragma once
#include "SDL_render.h"
#include "SDL_video.h"

void aoRun();
void aoQuit();

int aoGetW();
int aoGetH();
const char* aoGetTitle();

SDL_Window*& aoGetWindow();
SDL_Renderer*& aoGetRenderer();