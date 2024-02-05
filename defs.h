#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 576
#define TILESIZE 64
#define SPRITES_DIR "/data/sprite_mapping/"
#define PLAYER_SPEED 4
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

// entity flags
#define IS_SOLID 1
#define IS_PUSHABLE 2
#define IS_SWITCH 4
#define IS_RED 8
#define IS_BLUE 16
#define IS_GREEN 32
