#include <stdio.h>
#include <SDL2/SDL_image.h>
#include "basic_pixel.h"

#ifndef KERNEL_H
#define KERNEL_H

Uint8 kernel5x5(float a0,
                float a1,
                float a2,
                float a3,
                float a4,
                float b0,
                float b1,
                float b2,
                float b3,
                float b4,
                float c0,
                float c1,
                float c2,
                float c3,
                float c4,
                float d0,
                float d1,
                float d2,
                float d3,
                float d4,
                float e0,
                float e1,
                float e2,
                float e3,
                float e4,
                SDL_Surface *image, unsigned int i, unsigned int j);

Uint8 kernel3x3(float a0,
                float a1,
                float a2,
                float b0,
                float b1,
                float b2,
                float c0,
                float c1,
                float c2,
                SDL_Surface *image, unsigned int i, unsigned int j);

#endif
