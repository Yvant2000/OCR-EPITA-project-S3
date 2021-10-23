#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include "basic_pixel.h"


//Creates a 5X5 Kernel to be used in other functions,
//THe float values ave provided to set the mask
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
                SDL_Surface *image, unsigned int i, unsigned int j)
{
    return ((a0)*pixel_grey(image, i - 2, j - 2) + (a1)*pixel_grey(image, i - 1, j - 2) + 
            (a2)*pixel_grey(image, i, j - 2)     + (a3)*pixel_grey(image, i + 1, j - 2) + 
            (a4)*pixel_grey(image, i + 2, j - 2) +
            (b0)*pixel_grey(image, i - 2, j - 1) + (b1)*pixel_grey(image, i - 1, j - 1) +
            (b2)*pixel_grey(image, i, j - 1)     + (b3)*pixel_grey(image, i + 1, j - 1) + 
            (b4)*pixel_grey(image, i + 2, j - 1) + 
            (c0)*pixel_grey(image, i - 2, j)     + (c1)*pixel_grey(image, i - 1, j)     + 
            (c2)*pixel_grey(image, i, j)         + (c3)*pixel_grey(image, i + 1, j)     + 
            (c4)*pixel_grey(image, i + 2, j)     + 
            (d0)*pixel_grey(image, i - 2, j + 1) + (d1)*pixel_grey(image, i - 1, j + 1) + 
            (d2)*pixel_grey(image, i, j + 1)     + (d3)*pixel_grey(image, i + 1, j + 1) +
            (d4)*pixel_grey(image, i + 2, j + 1) + 
            (e0)*pixel_grey(image, i - 2, j + 2) + (e1)*pixel_grey(image, i - 1, j + 2) +
            (e2)*pixel_grey(image, i, j + 2)     + (e3)*pixel_grey(image, i + 1, j + 2) + 
            (e4)*pixel_grey(image, i + 2, j + 2)) /25;
}
//Creates a 3X3 Kernel to be used in other functions,
//THe float values ave provided to set the mask
Uint8 kernel3x3(float a0,
                float a1,
                float a2,
                float b0,
                float b1,
                float b2,
                float c0,
                float c1,
                float c2,
                SDL_Surface *image, unsigned int i, unsigned int j)
{
    return ((a0)*pixel_grey(image, i - 1, j - 1) + (a1)*pixel_grey(image, i, j - 1) + 
            (a2)*pixel_grey(image, i + 1, j - 1) +
            (b0)*pixel_grey(image, i - 1, j)     + (b1)*pixel_grey(image, i, j)     + 
            (b2)*pixel_grey(image, i + 1, j)     +
            (c0)*pixel_grey(image, i - 1, j + 1) + (c1)*pixel_grey(image, i, j + 1) + 
            (c2)*pixel_grey(image, i + 1, j + 1)) / 9;
}