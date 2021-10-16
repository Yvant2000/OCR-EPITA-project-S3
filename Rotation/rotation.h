#include <stdio.h>
#include <math.h>
#include <SDL.h>
#include <SDL_image.h>


#ifndef ROTATION_H
#define ROTATION_H

SDL_Surface *rotate(SDL_Surface *image, double angle);
int str_to_int(char *num);

#endif