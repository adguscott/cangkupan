
#include "common.h"
#include "main.h"
#include "init.h"
#include "draw.h"

Game game;
Sprites sprites;
SDL_Texture *spritesheet;
Player *player;

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

    player = initPlayer();

    while (1) {
        doInput();
	doMovement();
        drawScene();
        drawPlayer();
        SDL_RenderPresent(game.renderer);
        SDL_Delay(100);
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
	case SDL_KEYDOWN:
	    movePlayer(&event.key, 1);
	    break;
	case SDL_KEYUP:
	    movePlayer(&event.key, 0);
	    break;
	default:
	    break;
        }
    }
}

void movePlayer(SDL_KeyboardEvent *event, int down)
{
    if (event->repeat == 0) {
	if (event->keysym.scancode == SDL_SCANCODE_UP) {
	    player->dy = (down) ? player->dy - PLAYER_SPEED : 0;
	}

	if (event->keysym.scancode == SDL_SCANCODE_DOWN) {
	    player->dy = (down) ? player->dy + PLAYER_SPEED : 0;
	}

	if (event->keysym.scancode == SDL_SCANCODE_LEFT) {
	    player->dx = (down) ? player->dx - PLAYER_SPEED : 0;
	}

	if (event->keysym.scancode == SDL_SCANCODE_RIGHT) {
	    player->dx = (down) ? player->dx + PLAYER_SPEED : 0;
	}
    }
}

void doMovement(void)
{
    printf("x: %d, y: %d, dx: %d, dy: %d\n", player->x, player->y, player->dx, player->dy);
    player->x += player->dx;
    player->y += player->dy;
}

void drawScene(void)
{
    SDL_RenderClear(game.renderer);
    drawGround();
}

void drawGround(void)
{
    Sprite *groundSprite = getSprite("ground_06");
    for(int x = 0; x < SCREEN_WIDTH; x += 64) {
	for (int y = 0; y < SCREEN_HEIGHT; y += 64) {
	    blitRect(spritesheet, groundSprite, x, y);
	}
    }
}

void drawPlayer(void) 
{
    blitRect(spritesheet, player->sprite, player->x, player->y);
}
