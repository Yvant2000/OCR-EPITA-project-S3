#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>


#ifndef OTSU_BINARY_H
#define OTSU_BINARY_H

int Otsu(SDL_Surface* image_surface);
void OtsuBinarization(SDL_Surface* image, int threshold);
void Otsu1(SDL_Surface* image);

#endif
