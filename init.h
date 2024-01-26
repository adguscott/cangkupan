void initSDL(void);
Player *initPlayer(void);
void cleanup(void);
Sprite *createSpriteFromLine(char *line);
int loadSprites(void);
int loadSpriteFile(char *filename, Sprite *spriteTail);
int loadTexture(void);
Sprite *getSprite(char *spriteName);
