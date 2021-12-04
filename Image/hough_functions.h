#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef HOUGH_FUNCTIONS_H
#define HOUGH_FUNCTIONS_H

SDL_Surface *hough_transform(SDL_Surface *image,SDL_Surface *cleaned, char colour[PATH_MAX], char rot[PATH_MAX]);
void hough_alligned(SDL_Surface *image,double thresh);

#endif
