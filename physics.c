#include "common.h"
#include "physics.h"

void centreToTile(int *x, int *y, int width, int height, int nx, int ny)
{
    int startingX = (nx / TILESIZE) * TILESIZE;
    *x = startingX + getCentre(width);

    int startingY = (ny / TILESIZE) * TILESIZE;
    *y = startingY + getCentre(height);
}

int getCentre(int dim)
{
    if (dim >= TILESIZE)
	return dim;

    return (TILESIZE - dim) / 2;
}

// stolen from parallelrealities.co.uk
bool collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
	return (MAX(x1, x2) < MIN(x1 + w1, x2 + w2)) && (MAX(y1, y2) < MIN(y1 + h1, y2 + h2));
}

bool playerMoving(Player *player)
{
    return player->x != player->nx || player->y != player->ny;
}

bool entityMoving(Entity *entity)
{
    return entity->x != entity->nx || entity->y != entity->ny;
}

bool switched(Entity *crate, Entity *s)
{
    return crate->x <= s->x
	&& (crate->x + crate->sprite->w) >= (s->x + s->sprite->w)
	&& crate->y <= s->y
	&& (crate->y + crate->sprite->h) >= (s->y + s->sprite->h);
}
