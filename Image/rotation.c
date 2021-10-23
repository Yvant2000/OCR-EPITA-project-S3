#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "basic_pixel.h"
#include <math.h>

SDL_Surface *trim(SDL_Surface *image)
{
    size_t h = image->h;
    size_t w = image->w;
    size_t bottom_trim = 0;
    size_t top_trim = 0;
    size_t right_trim = 0;
    size_t left_trim = 0;

    for (size_t j = h; j > 0; j--)
    {
        for (size_t i = 0; i < w; i++)
        {
            Uint32 pix = get_pixel(image, i, j);
            //If the pixel is not black
            if (pix != SDL_MapRGB(image->format, 0, 0, 0))
            {
                //trim here on the bottom
                bottom_trim = j - 1;
            }
        }
    }
    for (size_t j = 0; j < h; j++)
    {
        for (size_t i = 0; i < w; i++)
        {
            Uint32 pix = get_pixel(image, i, j);
            //If the pixel is not black
            if (pix != SDL_MapRGB(image->format, 0, 0, 0))
            {
                //trim here on the top
                top_trim = j - 1;
            }
        }
    }
    for (size_t i = 0; i < w; i++)
    {
        for (size_t j = top_trim; j > bottom_trim; j--)
        {
            Uint32 pix = get_pixel(image, i, j);
            //If the pixel is not black
            if (pix != SDL_MapRGB(image->format, 0, 0, 0))
            {
                //trim here on the right
                right_trim =  i - 1;
            }
        }
    }
    for (size_t i = w; i > 0; i--)
    {
        for (size_t j = top_trim; j > bottom_trim; j--)
        {
            Uint32 pix = get_pixel(image, i, j);
            //If the pixel is not black
            if (pix != SDL_MapRGB(image->format, 0, 0, 0))
            {
                //trim here on the left
                left_trim =  i - 1;
            }
        }
    }
    SDL_Surface *output_image = SDL_CreateRGBSurface(0, w-(right_trim-left_trim), h - (top_trim-bottom_trim), 32, 0, 0, 0, 0);
    for (size_t i = left_trim; i < right_trim; i++)
    {
        for (size_t j = bottom_trim; j < top_trim; j++)
        {
            Uint32 pix = get_pixel(image, i, j);
            //If the pixel is not black
            put_pixel(output_image,i-left_trim,j-bottom_trim,pix);
        }
    }
    return output_image;
}
double my_abs(double value)
{
    if(value < 0.)
    {
     value *= -1.0;
     return value;
    }
    else
    {
     return value;
    }
}

int *new_sheer(double angle, int x, int y)
{
    int *x_and_y = malloc(sizeof(int) * 2);
    //sheer 1
    double intan = angle / 2.f;
    double tangent = tan(intan);
    double new_x = round((double)x - ((double)y * tangent));
    double new_y = y;
    //sheer 2
    new_y = round(new_x * sin(angle) + new_y);
    //sheer 3
    new_x = round(new_x - (new_y * tangent));

    x_and_y[0] = new_x;
    x_and_y[1] = new_y;
    return x_and_y;
}

SDL_Surface *rotate(SDL_Surface *image, double angle)
{

    if(angle == -1){
        return image;
    }

    int *x_and_y = malloc(sizeof(int) * 2);
    double cosine = cos(angle);
    double sinus = sin(angle);
    double height = image->h;
    double width = image->w;
    int new_height = round(my_abs(height * cosine) + 
		    my_abs(width * sinus)) + 1;
    int new_width = round(my_abs(width * cosine) +
		    my_abs(height * sinus)) + 1;
    SDL_Surface *output_image = SDL_CreateRGBSurface(0,
		    new_width * 2, new_height * 2, 32, 0, 0, 0, 0);
    int original_center_height = round(((height + 1) / 2) - 1);

    int original_center_width = round(((width + 1) / 2) - 1);
    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            int y = height - 1 - i - original_center_height;
            int x = width - 1 - j - original_center_width;
            x_and_y = new_sheer(angle, x, y);
            int new_x = x_and_y[0];
            int new_y = x_and_y[1];
            new_y = new_height - new_y;
            new_x = new_width - new_x;
            Uint32 pixel = get_pixel(image, i, j);
            put_pixel(output_image, new_y, new_x, pixel);
        }
    }
    free(x_and_y);
    SDL_Surface *test = trim(output_image);
    return test;
}


SDL_Surface *rotation_90(SDL_Surface *image)
{
    int w = image->w;
    int h = image->h;
    SDL_Surface *rotated = SDL_CreateRGBSurface(0, h, w, 32, 0, 0, 0, 0);
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            Uint32 pix = get_pixel(image, i, j);
            put_pixel(rotated, h - j - 1, i, pix);
        }
    }
    return rotated;
}

SDL_Surface *rotation_180(SDL_Surface *image)
{
    image = rotation_90(image);
    image = rotation_90(image);
    return image;
}

SDL_Surface *rotation_270(SDL_Surface *image)
{
    int w = image->w;
    int h = image->h;
    SDL_Surface *rotated = SDL_CreateRGBSurface(0, h, w, 32, 0, 0, 0, 0);
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            Uint32 pix = get_pixel(image, i, j);
            put_pixel(rotated, j, w - 1 - i, pix);
        }
    }
    return rotated;
}
