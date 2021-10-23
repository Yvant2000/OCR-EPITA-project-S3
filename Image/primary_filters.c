
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "basic_pixel.h"
#include "kernel.h"

SDL_Surface *gaussian_blur(SDL_Surface *image)
{
    int h = image->h;
    int w = image->w;
    SDL_Surface *image_coppy = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    for (int i = 1; i < w - 1; i++)
    {
        for (int j = 1; j < h - 1; j++)
        {
            Uint8 new_value = kernel3x3(
                0.8, 1, 0.8,
                1, 1.2, 1,
                0.8, 1, 0.8,
                image, i, j);
            Uint32 pixel_new = SDL_MapRGB(image->format, new_value, new_value, new_value);
            put_pixel(image_coppy, i, j, pixel_new);
        }
    }
    return image_coppy;
}

void grey_scale(SDL_Surface *image)
{
    int w = image->w;
    int h = image->h;
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            Uint32 pixel = get_pixel(image, i, j);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            Uint8 average = 0.3 * r + 0.59 * g + 0.11 * b;
            Uint32 pixel_new = SDL_MapRGB(image->format, average, average, average);
            put_pixel(image, i, j, pixel_new);
        }
    }
}