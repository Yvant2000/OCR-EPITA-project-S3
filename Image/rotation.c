#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "basic_pixel.h"
#include <math.h>

SDL_Surface *trim(SDL_Surface *image)
{
    // printf("Entered function\n");
    int h = image->h;
    int w = image->w;
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
    // printf("Bottom -> %zu\n",bottom_trim);
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
    // printf("Top -> %zu\n",top_trim);
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
    // printf("Right -> %zu\n",right_trim);
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
    // printf("Left -> %zu\n",left_trim);
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

int *new_sheer(double angle, int x, int y)
{
    int *x_and_y = malloc(sizeof(int) * 2);
    //sheer 1
    double intan = angle / 2.f;
    // printf("intan -> %.6f\n",intan);
    double tangent = tan(intan);
    // printf("tangent for angel -> %f: is -> %.6f\n",angle,tangent);
    int new_x = round((double)x - ((double)y * tangent));
    // printf("new_x -> %d \n",new_x);
    int new_y = y;
    // printf("new_y -> %d \n",new_y);
    //sheer 2
    new_y = round((double)new_x * sin(angle) + (double)new_y);
    // printf("new_y -> %d \n",new_y);
    //sheer 3
    new_x = round((double)new_x - ((double)new_y * tangent));
    // printf("new_x -> %d \n",new_x);
    x_and_y[0] = new_x;
    x_and_y[1] = new_y;
    return x_and_y;
}

SDL_Surface *rotate(SDL_Surface *image, double angle)
{

    printf("%f\n", angle);

    if(angle == -1){
        return image;
    }

    int *x_and_y = malloc(sizeof(int) * 2);
    double cosine = cos(angle);
    printf("cos -> %f\n",cosine);
    double sinus = sin(angle);
    printf("sin -> %f\n",sinus);
    int height = image->h;
    int width = image->w;
    int new_height = round((abs(height * cosine) + abs(width * sinus))) + 1;
    printf("new_h -> %d\n",new_height);
    int new_width = round((abs(width * cosine) + abs(height * sinus))) + 1;
    printf("new_w -> %d\n",new_width);
    SDL_Surface *output_image = SDL_CreateRGBSurface(0, new_width * 2, new_height * 2, 32, 0, 0, 0, 0);
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
    int w = image->w;
    int h = image->h;
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
