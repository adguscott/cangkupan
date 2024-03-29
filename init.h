void cleanup(void);
Sprite *createSpriteFromLine(char *line);
Sprite *getSprite(char *spriteName);
void initEntity(char *spriteName, int flags, int x, int y);
void initPlayer(int x, int y);
bool initSDL(void);
bool loadSprites(void);
bool loadSpriteFile(SpriteTuple *tuple);
bool loadTexture(void);
void setWindowIcon(void);
void setMainMenu(void);
void initMap(void);
void loadEntity(char entChar, int x, int y);
bool loadMap(void);
bool loadLevel(void);
void resetLevel(void);
void getScreenDims();
