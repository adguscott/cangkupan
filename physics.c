#include "physics.h"
#include "defs.h"

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
