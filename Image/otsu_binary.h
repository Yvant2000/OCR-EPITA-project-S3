#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


#ifndef OTSU_BINARY_H
#define OTSU_BINARY_H

int Otsu(SDL_Surface* image_surface);
void OtsuBinarization(SDL_Surface* image, int threshold);
void Otsu1(SDL_Surface* image);
void histogram_spreading(SDL_Surface *image);
void histogram_equil(SDL_Surface *image);
void decompose(SDL_Surface *image, int n);
int thresholding(SDL_Surface* image, double t);
#endif
