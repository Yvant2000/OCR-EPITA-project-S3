#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef DETECT_LINES_H
#define DETECT_LINES_H

SDL_Surface *detect_motive_hor(SDL_Surface *image);
SDL_Surface *detect_motive_vert(SDL_Surface *image);
SDL_Surface *combine_detections(SDL_Surface *image, SDL_Surface *image1);
void convolve(SDL_Surface *image_0,SDL_Surface *image_1,SDL_Surface *image_2, SDL_Surface *image_3, SDL_Surface *image_4, SDL_Surface *place);

#endif
