#include "SDL_image.h"
#include "common.h"
#include "init.h"

extern Game game;
extern Level level;

bool initSDL(void)
{
	int rendererFlags, windowFlags;
	rendererFlags = SDL_RENDERER_ACCELERATED;

	windowFlags = 0;
	
	windowFlags |= SDL_WINDOW_RESIZABLE;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Couldn't initialise SDL: %s\n", SDL_GetError());
		return false;
	}

	IMG_Init(IMG_INIT_PNG);

	game.window = SDL_CreateWindow("Cangkupan", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);
	
	if (!game.window) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to open SDL Window: %s\n", SDL_GetError());
		return false;
	}

	// not sure what this does?
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	game.renderer = SDL_CreateRenderer(game.window, -1, rendererFlags);

	if (!game.renderer) {
		SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Failed to create SDL renderer: %s\n", SDL_GetError());
		return false;
	}

	setWindowIcon();
	//SDL_SetWindowFullscreen(game.window, SDL_WINDOW_FULLSCREEN);
	return true;
}

bool loadTexture(void)
{
	game.spritesheet = IMG_LoadTexture(game.renderer, "gfx/spritesheet.png");

	if (game.spritesheet == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load spritesheet texture\n");
		return false;
	}

	return true;
}
void setWindowIcon(void)
{
	SDL_Log("Setting window icon\n");
	SDL_Surface* icon = IMG_Load("gfx/icon.png");
	SDL_SetWindowIcon(game.window, icon);
}

bool loadSprites(void)
{
	bool loadStatus = true;
	int index = 0;

	SDL_Log("Loading sprite data...\n");

	size_t arrayLength = 5;
	SpriteTuple spritesToLoad[5] = {
		{ "data/sprite_mapping/blocks", game.sprites.blocksTail },
		{ "data/sprite_mapping/crates", game.sprites.cratesTail },
		{ "data/sprite_mapping/environment", game.sprites.envTail },
		{ "data/sprite_mapping/ground", game.sprites.groundTail },
		{ "data/sprite_mapping/player", game.sprites.playerTail }
	};

	while (loadStatus && index < arrayLength) {
		loadStatus = loadSpriteFile(&spritesToLoad[index++]);
	}

	return loadStatus;
}

bool loadSpriteFile(SpriteTuple* tuple)
{
	FILE* fp;
	int lineSize = 100;
	Sprite* sprite;
	char* line = malloc(lineSize);

	if (line == NULL)
	{
		return false;
	}

	fp = fopen(tuple->fileName, "r");

	if (fp == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error opening %s", tuple->fileName);
		return false;
	}

	while ((fgets(line, lineSize, fp)) != NULL) {
		sprite = createSpriteFromLine(line);

		if (sprite == NULL) {
			free(line);
			return false;
		}

		tuple->spriteTail->next = sprite;
		tuple->spriteTail = sprite;
	}

	free(line);
	return true;
}

Sprite* createSpriteFromLine(char* line)
{
	int parseResult = 0;
	Sprite* sprite = malloc(sizeof(Sprite));
	if (sprite == NULL)
	{
		return NULL;
	}

	memset(sprite, 0, sizeof(Sprite));

	parseResult = sscanf(line, "%s %d %d %d %d", sprite->name, &sprite->x, &sprite->y, &sprite->w, &sprite->h);

	if (parseResult < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error loading sprite from line:\n%s\n", line);
		free(sprite);
		return NULL;
	}

	SDL_Log("Loading sprite: %s\n", sprite->name);
	return sprite;
}

void cleanup(void)
{
	SDL_DestroyRenderer(game.renderer);
	SDL_DestroyWindow(game.window);

	IMG_Quit();
	SDL_Quit();
}

void resetLevel(void)
{
	Entity* e;

	while (level.entities.entityHead.next) {
		e = level.entities.entityHead.next;
		level.entities.entityHead.next = e->next;
		free(e);
	}

	level.entities.entityTail = &level.entities.entityHead;
	level.switches = 0;
	loadMap();
}
Sprite* getSprite(char* spriteName)
{
	Sprite* sprite;

	for (sprite = game.sprites.blocksHead.next; sprite != NULL; sprite = sprite->next) {
		if (strcmp(sprite->name, spriteName) == 0) {
			return sprite;
		}
	}

	for (sprite = game.sprites.cratesHead.next; sprite != NULL; sprite = sprite->next) {
		if (strcmp(sprite->name, spriteName) == 0) {
			return sprite;
		}
	}

	for (sprite = game.sprites.groundHead.next; sprite != NULL; sprite = sprite->next) {
		if (strcmp(sprite->name, spriteName) == 0) {
			return sprite;
		}
	}

	for (sprite = game.sprites.envHead.next; sprite != NULL; sprite = sprite->next) {
		if (strcmp(sprite->name, spriteName) == 0) {
			return sprite;
		}
	}

	for (sprite = game.sprites.playerHead.next; sprite != NULL; sprite = sprite->next) {
		if (strcmp(sprite->name, spriteName) == 0) {
			return sprite;
		}
	}

	return NULL;
}

void initPlayer(int x, int y)
{
	SDL_Log("Initialising player...");
	Player* player = malloc(sizeof(Player));

	if (player == NULL) {
		return;
	}

	memset(player, 0, sizeof(Player));

	player->sprite = getSprite("player_05");
	player->animationDown[0] = getSprite("player_06");
	player->animationDown[1] = getSprite("player_05");
	player->animationDown[2] = getSprite("player_07");
	player->animationDown[3] = getSprite("player_05");

	player->animationUp[0] = getSprite("player_09");
	player->animationUp[1] = getSprite("player_08");
	player->animationUp[2] = getSprite("player_10");
	player->animationUp[3] = getSprite("player_08");

	player->animationLeft[0] = getSprite("player_21");
	player->animationLeft[1] = getSprite("player_20");
	player->animationLeft[2] = getSprite("player_22");
	player->animationLeft[3] = getSprite("player_20");

	player->animationRight[0] = getSprite("player_18");
	player->animationRight[1] = getSprite("player_17");
	player->animationRight[2] = getSprite("player_19");
	player->animationRight[3] = getSprite("player_17");

	player->x = x + ((TILESIZE - player->sprite->w) / 2);
	player->y = y + ((TILESIZE - player->sprite->h) / 2);

	player->nx = player->x;
	player->ny = player->y;

	if (level.player) {
		free(level.player);
	}

	level.player = player;
}

void initEntity(char* spriteName, int flags, int x, int y)
{
	Entity* entity = malloc(sizeof(Entity));

	if (entity == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Cannot malloc entity.");
		return;
	}

	memset(entity, 0, sizeof(Entity));

	entity->sprite = getSprite(spriteName);

	if (entity->sprite->w < TILESIZE) {
		entity->x = entity->nx = x + ((TILESIZE - entity->sprite->w) / 2);
	}
	else {
		entity->x = entity->nx = x;
	}

	if (entity->sprite->h < TILESIZE) {
		entity->y = entity->ny = y + ((TILESIZE - entity->sprite->h) / 2);
	}
	else {
		entity->y = entity->ny = y;
	}

	entity->flags = flags;
	level.entities.entityTail->next = entity;
	level.entities.entityTail = entity;

}

void initMap(void)
{
	char map[8][9] = { { '0', '1', '1', '1', '1', '1', '1', '1', '1' },
			   { '0', '1', '.', '1', '.', '0', '*', '0', '1' },
			   { '1', '1', 'p', '1', '1', '1', '0', '0', '1' },
			   { '1', '0', '*', '0', '1', '0', 'x', '0', '1' },
			   { '1', '0', '0', '*', '*', '.', '*', '.', '1' },
			   { '1', '0', '0', '.', '0', '0', '*', '0', '1' },
			   { '1', '1', '1', '1', '1', '1', '.', '0', '1' },
			   { '0', '0', '0', '0', '0', '1', '1', '1', '1' }
	};

	memcpy(level.map, map, sizeof(map));
}

bool loadLevel(void)
{
	initMap();
	return loadMap();
}

bool loadMap(void)
{
	int x, y;
	x = y = 0;
	for (int i = 0; i < 8; i++) {
		y = 0;
		for (int j = 0; j < 9; j++) {
			loadEntity(level.map[i][j], x, y);
			y += TILESIZE;
		}
		x += TILESIZE;
	}
	return true;
}

void loadEntity(char entChar, int x, int y)
{
	switch (entChar) {
	case '0':
		return;
		break;
	case '1':
		initEntity("block_08", IS_SOLID, x, y);
		break;
	case '*':
		initEntity("crate_02", IS_SOLID | IS_PUSHABLE, x, y);
		break;
	case 'x':
		initEntity("crate_02", IS_SOLID | IS_PUSHABLE, x, y);
	case '.':
		initEntity("environment_02", IS_SWITCH, x, y);
		level.switches += 1;
		break;
	case 'p':
		initPlayer(x, y);
		break;
	default:
		return;
	}
}

