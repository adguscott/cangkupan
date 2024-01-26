#include <SDL2/SDL_image.h>
#include "common.h"
#include "init.h"

extern Game game;
extern Sprites sprites;
extern SDL_Texture *spritesheet;

void initSDL(void) 
{
    int rendererFlags, windowFlags;
    rendererFlags = SDL_RENDERER_ACCELERATED;

    windowFlags = 0;
 
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Couldn't initialise SDL: %s\n", SDL_GetError());
        exit(1);
    }

    IMG_Init(IMG_INIT_PNG);

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

int loadTexture(void) {
    spritesheet = IMG_LoadTexture(game.renderer, "gfx/spritesheet.png");

    if (spritesheet == NULL) {
        perror("Failed to load spritesheet texture");
        return -1;
    }

    return 0;
}

int loadSprites(void)
{
    // TODO: fix this horrible function
    int loadStatus = 0;

    printf("Loading sprite data...\n");

    loadStatus = loadSpriteFile("data/sprite_mapping/blocks", sprites.blocksTail);

    if (loadStatus == -1)
        return -1;
    
    loadStatus = loadSpriteFile("data/sprite_mapping/crates", sprites.cratesTail);

    if (loadStatus == -1)
        return -1;

    loadStatus = loadSpriteFile("data/sprite_mapping/environment", sprites.envTail);

    if (loadStatus == -1)
        return -1;

    loadStatus = loadSpriteFile("data/sprite_mapping/ground", sprites.groundTail);

    if (loadStatus == -1)
        return -1;
    
    loadStatus = loadSpriteFile("data/sprite_mapping/player", sprites.playerTail);
    
    return loadStatus;
}

int loadSpriteFile(char *filename, Sprite *spriteTail)
{
    FILE *fp;
    char *line = NULL;
    size_t lineSize = 100;
    Sprite *sprite;
    
    fp = fopen(filename, "r");

    if (fp == NULL) {
       fprintf(stderr, "Error opening %s", filename); 
       return -1;
    }
    
    while ((getline(&line, &lineSize, fp)) != -1) {
        sprite = createSpriteFromLine(line);

        if (sprite == NULL) {
            return -1;
        }

        spriteTail->next = sprite;
        spriteTail = sprite; 
    }

    return 0;
}

Sprite *createSpriteFromLine(char *line)
{
    int parseResult = 0;
    Sprite *sprite = malloc(sizeof(Sprite));
    memset(sprite, 0, sizeof(Sprite));
   
    parseResult = sscanf(line, "%s %d %d %d %d", sprite->name, &sprite->x, &sprite->y, &sprite->w, &sprite->h);
    
    if (parseResult < 0) {
        perror("Could not parse sprite");
        free(sprite);
        return NULL;
    }
    
    printf("Loading sprite: %s\n", sprite->name);
    return sprite;
}

void cleanup(void)
{
    SDL_DestroyRenderer(game.renderer);

    SDL_DestroyWindow(game.window);
    
    IMG_Quit();
    SDL_Quit();
}

Sprite *getSprite(char *spriteName)
{
    Sprite *sprite;
    
    for (sprite = sprites.blocksHead.next; sprite != NULL; sprite = sprite->next) {
	if (strcmp(sprite->name, spriteName) == 0) {
	    return sprite;
	}
    }

    for (sprite = sprites.cratesHead.next; sprite != NULL; sprite = sprite->next) {
	if (strcmp(sprite->name, spriteName) == 0) {
	    return sprite;
	}
    }

    for (sprite = sprites.groundHead.next; sprite != NULL; sprite = sprite->next) {
	if (strcmp(sprite->name, spriteName) == 0) {
	    return sprite;
	}
    }
    
    for (sprite = sprites.envHead.next; sprite != NULL; sprite = sprite->next) {
	if (strcmp(sprite->name, spriteName) == 0) {
	    return sprite;
	}
    }
    
    for (sprite = sprites.playerHead.next; sprite != NULL; sprite = sprite->next) {
	if (strcmp(sprite->name, spriteName) == 0) {
	    return sprite;
	}
    }
        
    return NULL;
}

Player *initPlayer(void)
{
    Player *player = malloc(sizeof(Player));
    memset(player, 0, sizeof(Player));

    player->x = 100;
    player->y = 100;
    player->sprite = getSprite("player_05");

    return player;
}
