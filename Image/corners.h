#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef CORNERS_H
#define CORNERS_H

SDL_Surface *crop_the_image(SDL_Surface *image, SDL_Surface *colored);
#endif