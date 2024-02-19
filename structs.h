typedef struct Sprite Sprite;
typedef struct Entity Entity;
typedef struct MainMenu MainMenu;

enum Mode 
{
    MAINMENU = 0,
    PLAYING = 1,
    PAUSED = 2,
};

enum MainMenuOptions
{
    START = 0,
    MAP_EDITOR = 1,
    OPTIONS = 2,
    CREDITS = 3,
    EXIT = 4,
};

struct Sprite
{
    char name[50];
    int x, y;
    int w, h;
    Sprite *next;
};

struct MainMenu
{
    SDL_Texture* logo;
    Mix_Music* music;
    char menuOptions[5][11];
    int animationTime;
    int selectedOption;
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
    bool fullScreenMode;
    TTF_Font* font;
    int screenWidth;
    int screenHeight;
    enum Mode mode;
    MainMenu mainMenu;
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
    char **map;
    int mapWidth;
    int mapHeight;
    Player *player;
    Sprite *groundSprite;
    int switchCount;
    Entities entities;
    bool mapEditorMode;
    bool complete;
} Level;

typedef struct
{
    char fileName[200];
    Sprite *spriteTail;
} SpriteTuple;
