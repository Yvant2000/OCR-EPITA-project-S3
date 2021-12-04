#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


#ifndef OTSU_BINARY_H
#define OTSU_BINARY_H


void histogram_spreading(SDL_Surface *image);
void histogram_equil(SDL_Surface *image);
void decompose(SDL_Surface *image, int n);
int thresholding(SDL_Surface* image, double t);
double big_hist(SDL_Surface *image);
#endif
