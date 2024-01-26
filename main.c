#include "common.h"
#include "main.h"
#include "init.h"
#include "draw.h"

Game game;
Sprites sprites;
SDL_Texture *spritesheet;

int main(void)
{
    memset(&game, 0, sizeof(Game));
    memset(&sprites, 0, sizeof(Sprites));

    sprites.blocksTail = &sprites.blocksHead;
    sprites.cratesTail = &sprites.cratesHead;
    sprites.groundTail = &sprites.groundHead;
    sprites.envTail = &sprites.envHead;
    sprites.playerTail = &sprites.playerHead;

    atexit(&cleanup);
    initSDL();
    

    if (loadTexture() != 0) {
        fprintf(stderr, "Could not load spritesheet, exiting game.\n");
        cleanup();
        return -1;
    }

    if (loadSprites() != 0) {
        fprintf(stderr, "Could not load sprite data, exiting game.\n");
        cleanup();
        return -1;
    }

    printf("Sprites loaded\n");

    while (1) {
        doInput();
        drawScene();
        drawPlayer();
        SDL_RenderPresent(game.renderer);
        SDL_Delay(300);
    }
}

void doInput(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                exit(0);
                break;

            default:
                break;
        }
    }
}

void drawScene(void)
{
    SDL_SetRenderDrawColor(game.renderer, 128, 192, 255, 255);
    SDL_RenderClear(game.renderer);
}

void drawPlayer(void) 
{
    Sprite *sprite = NULL;
    int x, y;
    x = y = 0;

    for (sprite = sprites.blocksHead.next; sprite != NULL; sprite = sprite->next) {
            if (x + sprite->w > SCREEN_WIDTH) {
                y += 64;
                x = 0;
            }

            blitRect(spritesheet, sprite, x, y);
            x += 64;
    }

    for (sprite = sprites.cratesHead.next; sprite != NULL; sprite = sprite->next) {
            if (x + sprite->w > SCREEN_WIDTH) {
                y += 64;
                x = 0;
            }

            blitRect(spritesheet, sprite, x, y);
            x += 64;
    }

    for (sprite = sprites.groundHead.next; sprite != NULL; sprite = sprite->next) {
            if (x + sprite->w > SCREEN_WIDTH) {
                y += 64;
                x = 0;
            }

            blitRect(spritesheet, sprite, x, y);
            x += 64;
    }

    for (sprite = sprites.envHead.next; sprite != NULL; sprite = sprite->next) {
            if (x + sprite->w > SCREEN_WIDTH) {
                y += 64;
                x = 0;
            }

            blitRect(spritesheet, sprite, x, y);
            x += 64;
    }

    for (sprite = sprites.playerHead.next; sprite != NULL; sprite = sprite->next) {
            if (x + sprite->w > SCREEN_WIDTH) {
                y += 64;
                x = 0;
            }

            blitRect(spritesheet, sprite, x, y);
            x += 64;
}
}
