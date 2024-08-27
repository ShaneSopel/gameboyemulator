#ifndef UI_H
#define UI_H

#include <common.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

static const int SCREEN_WIDTH = 1024;
static const int SCREEN_HEIGHT = 768;

void ui_init();
void ui_handle_events();

void update_dbg_Window();
void delay(u32 ms);
u32 get_ticks();
void ui_update();
void display_tile(SDL_Surface *surface, u16 startLocation, u16 tileNum, int x, int y);

#endif