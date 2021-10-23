#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef DETECT_LINES_H
#define DETECT_LINES_H

SDL_Surface *detect_motive_hor(SDL_Surface *image);
SDL_Surface *detect_motive_vert(SDL_Surface *image);
void combine_detections(SDL_Surface *image, SDL_Surface *image1);

#endif