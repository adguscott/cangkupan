typedef struct Sprite Sprite;
typedef struct Entity Entity;
typedef struct Button Button;

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
} Game;

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

struct Button
{
    int x, y;
    Sprite *sprite;
    bool switched;
    Button *next;
};

typedef struct 
{
    Button buttonHead, *buttonTail;
} Buttons;

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
    Sprite *groundSprite;
} Level;

typedef struct
{
    char fileName[200];
    Sprite *spriteTail;
} SpriteTuple;
