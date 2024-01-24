#include "common.h"
#include "init.h"

extern Game game;
extern Sprite sprites;

void initSDL(void) 
{
    int rendererFlags, windowFlags;
    rendererFlags = SDL_RENDERER_ACCELERATED;

    windowFlags = 0;

    if (SDL_Init(SDL_INIT_VIDEO < 0)) {
        fprintf(stderr, "Couldn't initialise SDL: %s\n", SDL_GetError());
        exit(1);
    }

    game.window = SDL_CreateWindow("Cangkupan", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);
    if (!game.window) {
        fprintf(stderr, "Failed to open SDL Window: %s\n", SDL_GetError());
        exit(1);
    }
  
    // not sure what this does?
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    game.renderer = SDL_CreateRenderer(game.window, -1, rendererFlags);

    if (!game.renderer) {
        fprintf(stderr, "Failed to create SDL renderer: %s\n", SDL_GetError());
        exit(1);
    }
}

Sprite *createSpriteFromLine(char *line)
{
    Sprite *sprite = malloc(sizeof(Sprite));
    memset(sprite, 0, sizeof(Sprite));

    sprintf(line, "%s %d %d %d %d", sprite->name, sprite->rect.x, sprite->rect.y, sprite->rect.w, sprite->rect.h);

    return sprite;
}

void cleanup(void)
{
    SDL_DestroyRenderer(game.renderer);

    SDL_DestroyWindow(game.window);

    SDL_Quit();
}
