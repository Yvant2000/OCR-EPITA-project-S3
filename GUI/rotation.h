#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef ROTATION_H
#define ROTATION_H

SDL_Surface *rotate(SDL_Surface *image, double angle);
SDL_Surface *rotation_270(SDL_Surface *image);
SDL_Surface *rotation_90(SDL_Surface *image);
SDL_Surface *trim(SDL_Surface *image);
#endif
