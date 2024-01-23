#include "common.h"
#include "init.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

void initSDL(void) 
{
    int rendererFlags, windowFlags;
    rendererFlags = SDL_RENDERER_ACCELERATED;

    windowFlags = 0;

    if (SDL_Init(SDL_INIT_VIDEO < 0)) {
        fprintf(stderr, "Couldn't initialise SDL: %s\n", SDL_GetError());
        exit(1);
    }

    window = SDL_CreateWindow("Cangkupan", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);
    if (!window) {
        fprintf(stderr, "Failed to open SDL Window: %s\n", SDL_GetError());
        exit(1);
    }
  
    // not sure what this does?
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    renderer = SDL_CreateRenderer(window, -1, rendererFlags);

    if (!renderer) {
        fprintf(stderr, "Failed to create SDL renderer: %s\n", SDL_GetError());
        exit(1);
    }
}

void cleanup(void)
{
    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);

    SDL_Quit();
}
