#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef MEDIAN_FILTERS_H
#define MEDIAN_FILTERS_H

SDL_Surface *median( SDL_Surface *image);
SDL_Surface *median5( SDL_Surface *image);


#endif