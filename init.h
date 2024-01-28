void initSDL(void);
Player *initPlayer(void);
void cleanup(void);
Sprite *createSpriteFromLine(char *line);
bool loadSprites(void);
bool loadSpriteFile(SpriteTuple *tuple);
bool loadTexture(void);
Sprite *getSprite(char *spriteName);
