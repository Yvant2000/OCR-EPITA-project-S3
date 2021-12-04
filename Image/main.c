#define _GNU_SOURCE
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "decompose_image.h"

int main(int argc, char *argv[])
{
    apply_solve(argv[argc - 1]);
    return 0;
}
