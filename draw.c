#include "common.h"
#include "draw.h"

extern Game game;

void blitRect(SDL_Texture *texture, Sprite *src, int x, int y)
{
    SDL_Rect source, dest;
    
    source.x = src->x;
    source.y = src->y;
    source.w = src->w;
    source.h = src->h;

    dest.x = x;
    dest.y = y;
    dest.w = src->w;
    dest.h = src->h;

    SDL_RenderCopy(game.renderer, texture, &source, &dest);
}

