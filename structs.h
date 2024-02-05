typedef struct Sprite Sprite;
typedef struct Entity Entity;
typedef struct Button Button;

struct Sprite
{
    char name[50];
    int x, y;
    int w, h;
    Sprite *next;
};

typedef struct
{
    Sprite  blocksHead, *blocksTail;
    Sprite  cratesHead, *cratesTail;
    Sprite  groundHead, *groundTail;
    Sprite  envHead, *envTail;
    Sprite  playerHead, *playerTail;
} Sprites;

typedef struct
{
    Sprites sprites;
    SDL_Texture *spritesheet;
    SDL_Window *window;
    SDL_Renderer *renderer;
} Game;


struct Entity
{
    int x, y;
    int nx, ny;
    int dx, dy;
    Sprite *sprite;
    long flags;
    Entity *next;
};

typedef struct
{
   Entity entityHead, *entityTail; 
} Entities;

typedef struct
{
    int x, y;
    int nx, ny;
    int dx, dy;
    Sprite *sprite;
    Sprite *animationUp[4];
    Sprite *animationDown[4];
    Sprite *animationLeft[4];
    Sprite *animationRight[4];
} Player;

typedef struct
{
    int switches;
    Player *player;
    char map[8][9];
    Sprite *groundSprite;
    int switchCount;
    Entities entities;
    bool mapEditorMode;
} Level;

typedef struct
{
    char fileName[200];
    Sprite *spriteTail;
} SpriteTuple;
