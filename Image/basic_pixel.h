#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef BASIC_PIXEL_H
#define BASIC_PIXEL_H

static inline Uint8 *pixel_ref(SDL_Surface *surf, unsigned x, unsigned y);

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);

Uint8 pixel_grey(SDL_Surface *image, unsigned int i, unsigned int j);

#endif