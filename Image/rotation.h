#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>

#ifndef ROTATION_H
#define ROTATION_H

SDL_Surface *rotate(SDL_Surface *image, double angle);
SDL_Surface *rotation_270(SDL_Surface *image);
SDL_Surface *rotation_90(SDL_Surface *image);
#endif