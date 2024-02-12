#include "common.h"
#include "main.h"
#include "init.h"
#include "draw.h"
#include "physics.h"

Game game;
Level level;
Sprite* darkCrate;
Sprite* lightCrate;

int main(void)
{
	long then;
	float remainder;

	memset(&game, 0, sizeof(Game));

	game.sprites.blocksTail = &game.sprites.blocksHead;
	game.sprites.cratesTail = &game.sprites.cratesHead;
	game.sprites.groundTail = &game.sprites.groundHead;
	game.sprites.envTail = &game.sprites.envHead;
	game.sprites.playerTail = &game.sprites.playerHead;

	memset(&level, 0, sizeof(Level));
	level.entities.entityTail = &level.entities.entityHead;

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

	loadLevel();

	level.groundSprite = getSprite("ground_06");
	darkCrate = getSprite("crate_12");
	lightCrate = getSprite("crate_02");

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
			if (event.key.keysym.scancode == SDL_SCANCODE_R) {
				resetLevel();
				return;
			}
			movePlayer(&event.key, 1);
			break;
		default:
			break;
		}
	}
}

void movePlayer(SDL_KeyboardEvent* event, int down)
{
	if (playerMoving(level.player)) {
		return;
	}

	if (event->repeat == 0) {
		if (event->keysym.scancode == SDL_SCANCODE_UP) {
			level.player->dy = (down) ? -PLAYER_SPEED : 0;
			level.player->dx = 0;
			level.player->ny = level.player->y - TILESIZE;
		}

		if (event->keysym.scancode == SDL_SCANCODE_DOWN) {
			level.player->dy = (down) ? PLAYER_SPEED : 0;
			level.player->dx = 0;
			level.player->ny = level.player->y + TILESIZE;
		}

		if (event->keysym.scancode == SDL_SCANCODE_LEFT) {
			level.player->dx = (down) ? -PLAYER_SPEED : 0;
			level.player->dy = 0;
			level.player->nx = level.player->x - TILESIZE;
		}

		if (event->keysym.scancode == SDL_SCANCODE_RIGHT) {
			level.player->dx = (down) ? PLAYER_SPEED : 0;
			level.player->dy = 0;
			level.player->nx = level.player->x + TILESIZE;
		}
	}
}

void doMovement(void)
{
	if (playerMoving(level.player)) {
		level.player->x += level.player->dx;
		level.player->y += level.player->dy;
	}
	else {
		level.player->dx = level.player->dy = 0;
	}

	checkCollision();
	outOfBounds();
}


void doEntities(void)
{
	Entity* e;
	int switched = 0;
	for (e = level.entities.entityHead.next; e != NULL; e = e->next) {
		if (!(e->flags & IS_PUSHABLE))
			continue;

		if (entityMoving(e)) {
			e->x += e->dx;
			e->y += e->dy;
		}
		else {
			e->dx = e->dy = 0;
		}

		if (onSwitch(e)) {
			switched += 1;
			e->sprite = darkCrate;
		}
		else {
			e->sprite = lightCrate;
		}

	}

	if (switched == level.switches) {
		exit(1);
	}
}

bool onSwitch(Entity* e)
{
	Entity* s;
	for (s = level.entities.entityHead.next; s != NULL; s = s->next) {
		if (!(s->flags & IS_SWITCH))
			continue;

		if (switched(e, s)) {
			return true;
		}
	}
	return false;
}
void checkCollision(void)
{
	bool collided = false;
	Entity* e;
	for (e = level.entities.entityHead.next; e != NULL; e = e->next) {
		collided = (collision(level.player->x, level.player->y,
			level.player->sprite->w, level.player->sprite->h,
			e->x, e->y, e->sprite->w, e->sprite->h));

		if (collided) {
			if (IS_SOLID & e->flags) {
				if (IS_PUSHABLE & e->flags) {
					if (!entityMoving(e)) {
						int nx = e->x;
						int ny = e->y;

						// update nx based on direction level.player moving
						if (level.player->dx > 0) {
							nx = e->nx + TILESIZE;
						}
						else if (level.player->dx < 0) {
							nx = e->nx - TILESIZE;
						}

						// update ny based on direction level.player moving
						if (level.player->dy > 0) {
							ny = e->ny + TILESIZE;
						}
						else if (level.player->dy < 0) {
							ny = e->ny - TILESIZE;
						}

						if (checkCanMove(e, nx, ny)) {
							e->dx = level.player->dx;
							e->dy = level.player->dy;
							e->nx = nx;
							e->ny = ny;
						}
						else {
							stopAndRecentre();
						}
					}

				}
				else {
					stopAndRecentre();
				}
			}
		}
	}
}

void stopAndRecentre(void)
{
	// if object IS_SOLID and !IS_PUSHABLE, stop level.player moving and recenter position
	if (level.player->dx != 0) {
		level.player->nx = level.player->x = level.player->x - level.player->dx;
		level.player->dx = 0;
	}
	else {
		level.player->ny = level.player->y = level.player->y - level.player->dy;
		level.player->dy = 0;
	}

	centreToTile(&level.player->x, &level.player->y, level.player->sprite->w, level.player->sprite->h, level.player->x, level.player->y);

	level.player->nx = level.player->x;
	level.player->ny = level.player->y;
}

bool checkCanMove(Entity* entity, int nx, int ny)
{
	bool canMove = true;
	Entity* e;

	for (e = level.entities.entityHead.next; e != NULL; e = e->next) {
		if (e == entity || !(e->flags & IS_SOLID)) {
			continue;
		}

		canMove = !collision(nx, ny, entity->sprite->w, entity->sprite->h, e->x, e->y, e->sprite->w, e->sprite->h);

		if (!canMove) {
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
	for (int x = 0; x < SCREEN_WIDTH; x += TILESIZE) {
		for (int y = 0; y < SCREEN_HEIGHT; y += TILESIZE) {
			blitRect(game.spritesheet, level.groundSprite, x, y);
		}
	}
}

void drawEntities(void)
{
	Entity* entity;
	for (entity = level.entities.entityHead.next; entity != NULL; entity = entity->next) {
		if (!(entity->flags & IS_SWITCH))
			continue;

		blitRect(game.spritesheet, entity->sprite, entity->x, entity->y);
	}

	for (entity = level.entities.entityHead.next; entity != NULL; entity = entity->next) {
		if (entity->flags & IS_SWITCH)
			continue;

		blitRect(game.spritesheet, entity->sprite, entity->x, entity->y);
	}


}

void drawPlayer(void)
{
	int animationSpeed = SDL_GetTicks() / 120;
	int index = animationSpeed % 4;

	if (level.player->dy > 0) {
		level.player->sprite = level.player->animationDown[index];
	}

	if (level.player->dy < 0) {
		level.player->sprite = level.player->animationUp[index];
	}

	if (level.player->dx > 0) {
		level.player->sprite = level.player->animationRight[index];
	}

	if (level.player->dx < 0) {
		level.player->sprite = level.player->animationLeft[index];
	}

	blitRect(game.spritesheet, level.player->sprite, level.player->x, level.player->y);
}

void capFrameRate(long* then, float* remainder)
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
	// if level.player goes beyond bounds of screen, reset position to nearest tile
	if (level.player->x + level.player->sprite->w > SCREEN_WIDTH) {
		centreToTile(&level.player->x, &level.player->y, level.player->sprite->w, level.player->sprite->h, SCREEN_WIDTH - TILESIZE, level.player->y);
		level.player->nx = level.player->x;
		level.player->ny = level.player->y;
	}

	if (level.player->x < 0) {
		centreToTile(&level.player->x, &level.player->y, level.player->sprite->w, level.player->sprite->h, 0, level.player->y);
		level.player->nx = level.player->x;
		level.player->ny = level.player->y;
	}

	if (level.player->y + level.player->sprite->h > SCREEN_HEIGHT) {
		centreToTile(&level.player->x, &level.player->y, level.player->sprite->w, level.player->sprite->h, level.player->x, SCREEN_HEIGHT - TILESIZE);
		level.player->nx = level.player->x;
		level.player->ny = level.player->y;
	}

	if (level.player->y < 0) {
		centreToTile(&level.player->x, &level.player->y, level.player->sprite->w, level.player->sprite->h, level.player->x, 0);
		level.player->nx = level.player->x;
		level.player->ny = level.player->y;
	}
}
