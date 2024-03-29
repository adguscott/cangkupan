bool collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
void centreToTile(int *x, int *y, int width, int height, int nx, int ny);
int coordToTileCoord(int coord);
int getCentre(int dim);
bool playerMoving(Player *player);
bool entityMoving(Entity *entity);
bool switched(Entity *crate, Entity *s);
