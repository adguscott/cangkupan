typedef struct Sprite Sprite;
typedef struct Entity Entity;

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
    int dx, dy;
    Sprite *sprite;
    long flags;
    Entity *next;
};

typedef struct
{
    int x, y;
    int dx, dy;
    Sprite *sprite;
    //Sprite *sprites[];
} Player;
