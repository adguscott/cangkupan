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
Level level;

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

    memset(&level, 0, sizeof(Level));
    
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
    
    level.groundSprite = getSprite("ground_06");
    
    player = initPlayer();
    initEntity("crate_02", IS_SOLID | IS_PUSHABLE, 192, 192);
    initEntity("crate_03", IS_SOLID, 320, 320);
    
    then = SDL_GetTicks();
    remainder = 0;
    
    while (true) {
        doInput();
	doMovement();
	doEntities();
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
    if (playerMoving(player)) {
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
    }
}

void doMovement(void)
{
    if (playerMoving(player)) {
	player->x += player->dx;
	player->y += player->dy;
    } else {
	player->dx = player->dy = 0;
    }

    checkCollision();
    outOfBounds();
 }

void doEntities(void)
{
    Entity *e;
    for (e = entities.entityHead.next; e != NULL; e = e->next) {
	if (entityMoving(e)) {
	    e->x += e->dx;
	    e->y += e->dy;
	} else {
	    e->dx = e->dy = 0;
	}
    }
}

void checkCollision(void)
{
    bool collided = false;
    Entity *e;
    for (e = entities.entityHead.next; e != NULL; e = e->next) {
	collided = (collision(player->x, player->y,
		      player->sprite->w, player->sprite->h,
				    e->x, e->y, e->sprite->w, e->sprite->h));
	
	if (collided) {
	    if (IS_SOLID & e->flags) {
		if (IS_PUSHABLE & e->flags) {
		    if (!entityMoving(e)) {
			int nx = e->x;
			int ny = e->y;
			
			// update nx based on direction player moving
			if (player->dx > 0) {
			    nx = e->nx + TILESIZE;
			} else if(player->dx < 0) {
			    nx = e->nx - TILESIZE;
			}

			// update ny based on direction player moving
			if (player->dy > 0) {
			    ny = e->ny + TILESIZE;
			} else if(player->dy < 0) {
			    ny = e->ny - TILESIZE;
			}

			if (checkCanMove(e, nx, ny)) {
			    e->dx = player->dx;
			    e->dy = player->dy;
			    e->nx = nx;
			    e->ny = ny;
			} else {
			    stopAndRecentre();
			}
		    }
		    
		} else {
		    stopAndRecentre();
		}
	    }
	}
    }
}

void stopAndRecentre(void)
{
    // if object IS_SOLID and !IS_PUSHABLE, stop player moving and recenter position
    if (player->dx != 0) {
	player->nx = player->x = player->x - player->dx;
	player->dx = 0;
    } else {
	player->ny = player->y = player->y - player->dy;
	player->dy = 0;
    }
    
    centreToTile(&player->x, &player->y, player->sprite->w, player->sprite->h, player->x, player->y);
    
    player->nx = player->x;
    player->ny = player->y;
}

bool checkCanMove(Entity *entity, int nx, int ny)
{
    bool canMove = true;
    Entity *e;

    for (e = entities.entityHead.next; e != NULL; e = e->next) {
	if (e == entity) {
	    continue;
	}
	
	canMove = !collision(nx, ny, entity->sprite->w, entity->sprite->h, e->x, e->y, e->sprite->w, e->sprite->h);

	if(!canMove) {
	    return false;
	}

    }
    canMove = !(nx + entity->sprite->w > SCREEN_WIDTH) && !(nx < 0) && !(ny + entity->sprite->h > SCREEN_HEIGHT) && !(ny < 0);
    
    return canMove;
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
    
    for (int y = TILESIZE; y < SCREEN_WIDTH; y += TILESIZE) {
	SDL_RenderDrawLine(game.renderer, 0, y, SCREEN_WIDTH, y);
    }

    for (int x = TILESIZE; x < SCREEN_WIDTH; x += TILESIZE) {
	SDL_RenderDrawLine(game.renderer, x, 0, x, SCREEN_HEIGHT);
    }
    
    SDL_SetRenderDrawBlendMode(game.renderer, SDL_BLENDMODE_NONE);
}

void drawGround(void)
{
    for(int x = 0; x < SCREEN_WIDTH; x += TILESIZE) {
	for (int y = 0; y < SCREEN_HEIGHT; y += TILESIZE) {
	    blitRect(spritesheet, level.groundSprite, x, y);
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
    int animationSpeed = SDL_GetTicks() / 120;
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
    //
    long wait, frameTime;
    
    wait = 16 + *remainder;
    
    *remainder -= (int)*remainder;

    frameTime = SDL_GetTicks() - *then;

    wait -= frameTime;

    if (wait < 1) {
	wait = 1;
    }

    SDL_Delay(wait);

    *remainder += 0.667;

    *then = SDL_GetTicks();
}

void outOfBounds(void)
{
    // if player goes beyond bounds of screen, reset position to nearest tile
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
