#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#ifndef PRIMARY_FILTERS_H
#define PRIMARY_FILTERS_H

SDL_Surface *gaussian_blur(SDL_Surface *image);
void grey_scale(SDL_Surface *image);

#endif