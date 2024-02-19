#include "common.h"
#include "draw.h"

extern Game game;

void blitRect(SDL_Texture* texture, Sprite* src, int x, int y)
{
	SDL_Rect source =
	{
		.x = src->x,
		.y = src->y,
		.w = src->w,
		.h = src->h,
	};
	
	SDL_Rect dest =
	{
		.x = x,
		.y = y,
		.w = src->w,
		.h = src->h,
	};
	
	SDL_RenderCopy(game.renderer, texture, &source, &dest);
}

void drawText(const char* text, const int fontSize, bool selected, bool centered, int x, int y)
{
	SDL_Rect dest;
	SDL_Color white =
	{
		.r = 255,
		.g = 255,
		.b = 255,
		.a = 255
	};
	
	SDL_Color yellow =
	{
		.r = 255,
		.g = 255,
		.b = 0,
		.a = 255
	};

	if (TTF_FontHeight != fontSize)
	{
		TTF_SetFontSize(game.font, fontSize);
	}

	SDL_Surface* surface = TTF_RenderUTF8_Blended(game.font, text, selected ? yellow : white);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(game.renderer, surface);
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

	if (centered) 
	{
		dest.x = (game.screenWidth - dest.w) / 2;
		dest.y = (game.screenHeight - dest.h) / 2;
	}
	else
	{
		dest.x = x;
		dest.y = y;
	}

	SDL_RenderCopy(game.renderer, texture, NULL, &dest);
}