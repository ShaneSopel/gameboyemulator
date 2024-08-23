#ifndef UI_H
#define UI_H

#include <common.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

static const int SCREEN_WIDTH = 1024;
static const int SCREEN_HEIGHT = 768;

void ui_init();
void ui_handle_events();

#endif