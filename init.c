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

bool loadTexture(void) {
    spritesheet = IMG_LoadTexture(game.renderer, "gfx/spritesheet.png");

    if (spritesheet == NULL) {
        perror("Failed to load spritesheet texture");
        return false;
    }

    return true;
}

bool loadSprites(void)
{
    bool loadStatus = true;
    int index = 0;
    
    
    printf("Loading sprite data...\n");

    size_t arrayLength = 5;
    SpriteTuple spritesToLoad[5] = {
	{ "data/sprite_mapping/blocks", sprites.blocksTail },
	{ "data/sprite_mapping/crates", sprites.cratesTail },
	{ "data/sprite_mapping/environment", sprites.envTail },
	{ "data/sprite_mapping/ground", sprites.groundTail },
	{ "data/sprite_mapping/player", sprites.playerTail }
    };

    while (loadStatus && index < arrayLength) {
	loadStatus = loadSpriteFile(&spritesToLoad[index++]);
    }
    
    return loadStatus;
}

bool loadSpriteFile(SpriteTuple *tuple)
{
    FILE *fp;
    char *line = NULL;
    size_t lineSize = 100;
    Sprite *sprite;
    
    fp = fopen(tuple->fileName, "r");

    if (fp == NULL) {
       fprintf(stderr, "Error opening %s", tuple->fileName); 
       return false;
    }
    
    while ((getline(&line, &lineSize, fp)) != -1) {
        sprite = createSpriteFromLine(line);

        if (sprite == NULL) {
            return false;
        }

        tuple->spriteTail->next = sprite;
        tuple->spriteTail = sprite; 
    }

    return true;
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
    

    return player;
}
