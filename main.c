#include "common.h"
#include "main.h"
#include "init.c"

int main(void)
{
    atexit(&cleanup);
    initSDL();

    while (1) {
        doInput();
    }
}

void doInput(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                exit(0);
                break;

            default:
                break;
        }
    }
}
