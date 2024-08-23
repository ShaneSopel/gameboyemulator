#include <emu.h>
#include <ui.h>

SDL_Window *sdlWindow;
SDL_Renderer *sdlRenderer;
SDL_Texture *sdlTexture;
SDL_Surface *screen;

void ui_init()
{
    SDL_Init(SDL_INIT_VIDEO);
    printf("SDL INITIATED \n");
    TTF_Init();
    printf("TTF INITIATED \n");

    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, 
    SCREEN_HEIGHT, 0, &sdlWindow, &sdlRenderer);
}

void ui_handle_events()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) > 0)
    {
        if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE)
        {
            emu_get_context()->die = true;
        }
    }

}