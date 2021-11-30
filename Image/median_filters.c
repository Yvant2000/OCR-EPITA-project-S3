#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "array_sort.h"
#include "basic_pixel.h"


//Application of median filter 3X3 pixels
//The median value is the fifth in a sorted list of 9 values
SDL_Surface *median( SDL_Surface *image)
{
    int w = image->w;
    int h = image->h;
    SDL_Surface *image_coppy = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    for (int i = 1; i < w - 1; i++)
    {
        for (int j = 1; j < h - 1; j++)
        {
            //A table taking all the pixels around the pixel (i,j)
            int table[9] = {pixel_grey(image, i - 1, j - 1), pixel_grey(image, i, j - 1), pixel_grey(image, i + 1, j - 1),
                            pixel_grey(image, i - 1, j), pixel_grey(image, i, j), pixel_grey(image, i + 1, j),
                            pixel_grey(image, i - 1, j + 1), pixel_grey(image, i, j + 1), pixel_grey(image, i + 1, j + 1)};
            array_select_sort(table, 9);
            // The median value here is at index 4
            int median_value = table[4];
            Uint32 pixel_new = SDL_MapRGB(image->format, median_value, median_value, median_value);
            put_pixel(image_coppy, i, j, pixel_new);
        }
    }
    return image_coppy;
}
//Application of median filter 5X5 pixels
//The median value is the twelvth in a sorted list of 25 values
SDL_Surface *median5(SDL_Surface *image)
{
    int w = image->w;
    int h = image->h;
    SDL_Surface *image_coppy = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    for (int i = 2; i < w - 2; i++)
    {
        for (int j = 2; j < h - 2; j++)
        {
            //A table taking all the pixels around the pixel (i,j)
            int table[25] = {pixel_grey(image, i - 1, j - 1), pixel_grey(image, i, j - 1), pixel_grey(image, i + 1, j - 1),
                             pixel_grey(image, i - 1, j), pixel_grey(image, i, j), pixel_grey(image, i + 1, j),
                             pixel_grey(image, i - 1, j + 1), pixel_grey(image, i, j + 1), pixel_grey(image, i + 1, j + 1),
                             pixel_grey(image, i - 2, j - 2), pixel_grey(image, i - 1, j - 2), pixel_grey(image, i, j - 2),
                             pixel_grey(image, i + 1, j - 2), pixel_grey(image, i + 2, j - 2), pixel_grey(image, i - 2, j - 1),
                             pixel_grey(image, i + 2, j - 1), pixel_grey(image, i - 2, j), pixel_grey(image, i + 2, j),
                             pixel_grey(image, i - 2, j + 1), pixel_grey(image, i + 2, j + 1),
                             pixel_grey(image, i - 2, j + 2), pixel_grey(image, i - 1, j + 2),
                             pixel_grey(image, i, j + 2), pixel_grey(image, i + 1, j + 2), pixel_grey(image, i + 2, j + 2)};
            array_select_sort(table, 25);
            //The median value here is at index 12
            int median_value = table[12];
            Uint32 pixel_new = SDL_MapRGB(image->format, median_value, median_value, median_value);
            put_pixel(image_coppy, i, j, pixel_new);
        }
    }
    return image_coppy;
}
