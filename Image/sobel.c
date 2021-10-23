#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include "basic_pixel.h"
#include "kernel.h"
#include <math.h>


SDL_Surface *horizontal_edge_detection_sobel(SDL_Surface *image)
{
    int w = image->w;
    int h = image->h;
    SDL_Surface *image_coppy = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    for (int i = 1; i < w - 1; i++)
    {
        for (int j = 1; j < h - 1; j++)
        {
            Uint8 new_value = kernel3x3(
                1, 2, 1,
                0, 0, 0,
                -1, -2, -1,
                image, i, j);
            Uint32 pixel_new = SDL_MapRGB(image->format, new_value, new_value, new_value);
            put_pixel(image_coppy, i, j, pixel_new);
        }
    }
    return image_coppy;
}
SDL_Surface *horizontal_edge_detection_sobel1(SDL_Surface *image)
{
    int w = image->w;
    int h = image->h;
    SDL_Surface *image_coppy = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    for (int i = 1; i < w - 1; i++)
    {
        for (int j = 1; j < h - 1; j++)
        {
            Uint8 new_value = kernel3x3(
                -1, -2, -1,
                0, 0, 0,
                1, 2, 1,
                image, i, j);
            Uint32 pixel_new = SDL_MapRGB(image->format, new_value, new_value, new_value);
            put_pixel(image_coppy, i, j, pixel_new);
        }
    }
    return image_coppy;
}
SDL_Surface *vertical_edge_detection_sobel(SDL_Surface *image)
{
    int w = image->w;
    int h = image->h;
    SDL_Surface *image_coppy = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    for (int i = 1; i < w - 1; i++)
    {
        for (int j = 1; j < h - 1; j++)
        {
            Uint8 new_value = kernel3x3(
                -1, 0, 1,
                -2, 0, 2,
                -1, 0, 1,
                image, i, j);
            Uint32 pixel_new = SDL_MapRGB(image->format, new_value, new_value, new_value);
            put_pixel(image_coppy, i, j, pixel_new);
        }
    }
    return image_coppy;
}
SDL_Surface *vertical_edge_detection_sobel1(SDL_Surface *image)
{
    int w = image->w;
    int h = image->h;
    SDL_Surface *image_coppy = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    for (int i = 1; i < w - 1; i++)
    {
        for (int j = 1; j < h - 1; j++)
        {
            Uint8 new_value = kernel3x3(
                1, 0, -1,
                2, 0, -2,
                1, 0, -1,
                image, i, j);
            Uint32 pixel_new = SDL_MapRGB(image->format, new_value, new_value, new_value);
            put_pixel(image_coppy, i, j, pixel_new);
        }
    }
    return image_coppy;
}



SDL_Surface *hysteris(SDL_Surface *image)
{
    int h = image->h;
    int w = image->w;
    SDL_Surface *image_coppy = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    for (size_t i = 1; i < w - 1; i++)
    {
        for (size_t j = 1; j < h - 1; j++)
        {
            Uint8 new_pix_val = pixel_grey(image, i, j);
            if (new_pix_val < 200)
            {
                if ((pixel_grey(image, i + 1, j - 1) >= 200) || (pixel_grey(image, i + 1, j) >= 200) ||
                    (pixel_grey(image, i + 1, j + 1) >= 200) || (pixel_grey(image, i, j - 1) >= 200) ||
                    (pixel_grey(image, i, j + 1) >= 200) || (pixel_grey(image, i - 1, j - 1) >= 200) ||
                    (pixel_grey(image, i - 1, j) >= 200) || (pixel_grey(image, i - 1, j + 1) >= 200))
                {
                    Uint32 new_pix = SDL_MapRGB(image->format, 0, 0, 0);
                    put_pixel(image_coppy, i, j, new_pix);
                }
                else
                {
                    Uint32 new_pix = SDL_MapRGB(image->format, 255, 255, 255);
                    put_pixel(image_coppy, i, j, new_pix);
                }
            }
        }
    }
    return image_coppy;
}

SDL_Surface *combine_sobel(SDL_Surface *image_hor, SDL_Surface *image_vert)
{
    int h = image_hor->h;
    int w = image_hor->w;
    SDL_Surface *image_coppy = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    for (size_t i = 0; i < w; i++)
    {
        for (size_t j = 0; j < h; j++)
        {
            Uint8 hor_pix = pixel_grey(image_hor, i, j);
            Uint8 vert_pix = pixel_grey(image_vert, i, j);
            int squared = vert_pix * vert_pix + hor_pix * hor_pix;
            int new_pix_val = (int)sqrt((double)squared);
            Uint32 new_pix = SDL_MapRGB(image_hor->format, new_pix_val, new_pix_val, new_pix_val);
            put_pixel(image_coppy, i, j, new_pix);
        }
    }
    return image_coppy;
}


SDL_Surface *sobel(SDL_Surface *image)
{
    int h = image->h;
    int w = image->w;
    SDL_Surface *image_hor = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    image_hor = horizontal_edge_detection_sobel(image);

    SDL_Surface *image_vert = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    image_vert = vertical_edge_detection_sobel(image);

    SDL_Surface *image_hor1 = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    image_hor1 = horizontal_edge_detection_sobel1(image);

    SDL_Surface *image_vert1 = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    image_vert1 = vertical_edge_detection_sobel1(image);

    SDL_Surface *sobel = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    sobel = combine_sobel(image_hor, image_vert);

    SDL_Surface *sobel1 = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    sobel1 = combine_sobel(image_hor1, image_vert1);

    SDL_Surface *sobel2 = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    sobel2 = combine_sobel(sobel1, sobel);

    SDL_FreeSurface(image_hor);
    SDL_FreeSurface(image_vert);
    SDL_FreeSurface(image_hor1);
    SDL_FreeSurface(image_vert1);
    SDL_FreeSurface(sobel);
    SDL_FreeSurface(sobel1);

    return sobel2;
}

