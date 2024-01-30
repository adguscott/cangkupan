#include "common.h"
#include "main.h"
#include "init.h"
#include "draw.h"
#include "physics.h"

Game game;
Sprites sprites;
SDL_Texture *spritesheet;
Player *player;
Entities entities;

int main(void)
{
    long then;
    float remainder;
    
    memset(&game, 0, sizeof(Game));
    memset(&sprites, 0, sizeof(Sprites));

    sprites.blocksTail = &sprites.blocksHead;
    sprites.cratesTail = &sprites.cratesHead;
    sprites.groundTail = &sprites.groundHead;
    sprites.envTail = &sprites.envHead;
    sprites.playerTail = &sprites.playerHead;

    memset(&entities, 0, sizeof(Entities));
    entities.entityTail = &entities.entityHead;
    
    atexit(&cleanup);

    if (!initSDL()) {
	cleanup();
	return EXIT_FAILURE;
    }
    
    if (!loadTexture()) {
        cleanup();
        return EXIT_FAILURE;
    }

    if (!loadSprites()) {
        cleanup();
        return EXIT_FAILURE;
    }

    player = initPlayer();
    initEntities();
    
    then = SDL_GetTicks();
    remainder = 0;
    
    while (true) {
        doInput();
	doMovement();
        drawScene();
	drawLines();
	drawEntities();
	drawPlayer();
	capFrameRate(&then, &remainder);
        SDL_RenderPresent(game.renderer);
    }

   return EXIT_SUCCESS;
}

void doInput(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
	case SDL_QUIT:
	    exit(EXIT_SUCCESS);
	    break;
	case SDL_KEYDOWN:
	    movePlayer(&event.key, 1);
	    break;
	default:
	    break;
        }
    }
}

void movePlayer(SDL_KeyboardEvent *event, int down)
{
    if (player->isMoving) {
	return;
    }
    
    if (event->repeat == 0) {
	if (event->keysym.scancode == SDL_SCANCODE_UP) {
	    player->dy = (down) ? -PLAYER_SPEED : 0;
	    player->dx = 0;
	    player->ny = player->y - TILESIZE;
	}

	if (event->keysym.scancode == SDL_SCANCODE_DOWN) {
	    player->dy = (down) ? PLAYER_SPEED : 0;
    	    player->dx = 0;
	    player->ny = player->y + TILESIZE;
	}

	if (event->keysym.scancode == SDL_SCANCODE_LEFT) {
	    player->dx = (down) ? -PLAYER_SPEED : 0;
	    player->dy = 0;
	    player->nx = player->x - TILESIZE;
	}

	if (event->keysym.scancode == SDL_SCANCODE_RIGHT) {
	    player->dx = (down) ? PLAYER_SPEED : 0;
	    player->dy = 0;
	    player->nx = player->x + TILESIZE;
	}
	
	player->isMoving = player->x != player->nx || player->y != player->ny;
    }
}

void doMovement(void)
{
    player->isMoving = player->x != player->nx || player->y != player->ny;
    if (player->isMoving) {
	player->x += player->dx;
	player->y += player->dy;
    } else {
	player->dx = player->dy = 0;
    }

    outOfBounds();
}

void drawScene(void)
{
    SDL_RenderClear(game.renderer);
    drawGround();
}

void drawLines(void)
{
    SDL_SetRenderDrawBlendMode(game.renderer, SDL_BLENDMODE_BLEND);
    
    SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 20);
    
    for (int y = 64; y < SCREEN_WIDTH; y += 64) {
	SDL_RenderDrawLine(game.renderer, 0, y, SCREEN_WIDTH, y);
    }

    for (int x = 64; x < SCREEN_WIDTH; x += 64) {
	SDL_RenderDrawLine(game.renderer, x, 0, x, SCREEN_HEIGHT);
    }
    
    SDL_SetRenderDrawBlendMode(game.renderer, SDL_BLENDMODE_NONE);
}

void drawGround(void)
{
    Sprite *groundSprite = getSprite("ground_06");
    for(int x = 0; x < SCREEN_WIDTH; x += TILESIZE) {
	for (int y = 0; y < SCREEN_HEIGHT; y += TILESIZE) {
	    blitRect(spritesheet, groundSprite, x, y);
	}
    }
}

void drawEntities(void)
{
    Entity *entity;
    for (entity = entities.entityHead.next; entity != NULL; entity = entity->next) {
	blitRect(spritesheet, entity->sprite, entity->x, entity->y);
    }
}

void drawPlayer(void) 
{
    int animationSpeed = SDL_GetTicks() / 100;
    int index = animationSpeed % 4;

    if (player->dy > 0) {
	player->sprite = player->animationDown[index];
    }

    if (player->dy < 0) {
       	player->sprite = player->animationUp[index];
    }

    if (player->dx > 0) {
	player->sprite = player->animationRight[index];
    }

    if (player->dx < 0) {
	player->sprite = player->animationLeft[index];
    }
    
    blitRect(spritesheet, player->sprite, player->x, player->y);
}

void capFrameRate(long *then, float *remainder)
{
	long wait, frameTime;

	wait = 16 + *remainder;

	*remainder -= (int)*remainder;

	frameTime = SDL_GetTicks() - *then;

	wait -= frameTime;

	if (wait < 1)
	{
		wait = 1;
	}

	SDL_Delay(wait);

	*remainder += 0.667;

	*then = SDL_GetTicks();
}

void outOfBounds(void)
{
    if (player->x + player->sprite->w > SCREEN_WIDTH) {
	centreToTile(&player->x, &player->y, player->sprite->w, player->sprite->h, SCREEN_WIDTH - TILESIZE, player->y);
	player->nx = player->x;
	player->ny = player->y;
    }

    if (player->x < 0) {
	centreToTile(&player->x, &player->y, player->sprite->w, player->sprite->h, 0, player->y);
	player->nx = player->x;
	player->ny = player->y;
    }

    if (player->y + player->sprite->h > SCREEN_HEIGHT) {
	centreToTile(&player->x, &player->y, player->sprite->w, player->sprite->h, player->x, SCREEN_HEIGHT - TILESIZE);
	player->nx = player->x;	
	player->ny = player->y;
    }

    if (player->y < 0) {
	centreToTile(&player->x, &player->y, player->sprite->w, player->sprite->h, player->x, 0);
	player->nx = player->x;	
	player->ny = player->y;
    }
}
