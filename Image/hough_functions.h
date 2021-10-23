#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>

#ifndef HOUGH_FUNCTIONS_H
#define HOUGH_FUNCTIONS_H
typedef struct Rotated
{
    SDL_Surface *image_output;
    double theta;
} Rotated;


Rotated *hough_transform(SDL_Surface *image, SDL_Surface *original);

#endif