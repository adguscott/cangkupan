typedef struct Sprite Sprite;

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
} Game;

struct Sprite
{
   char name[50];
   int x;
   int y;
   int w;
   int h;
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
