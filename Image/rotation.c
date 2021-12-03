#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "basic_pixel.h"
#include <math.h>



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
    SDL_FreeSurface(image);
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
    SDL_FreeSurface(image);
    return rotated;
}


void change_to_red(SDL_Surface *image)
{
    int h = image->h;
    int w = image->w;
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
           Uint32 pix = SDL_MapRGB(image->format,255,0,0);
           put_pixel(image,i,j,pix);
        }
    }
}

void change_red_to_white(SDL_Surface *image)
{
    int h = image->h;
    int w = image->w;
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            
           if(get_pixel(image,i,j) == SDL_MapRGB(image->format,255,0,0))
           {
               Uint32 pix = SDL_MapRGB(image->format,255,255,255);
               put_pixel(image,i,j,pix);
           }
        }
    }
}

void draw_border(SDL_Surface *image)
{
    int h = image->h;
    int w = image->w;
    for (int i = 0; i < w; i++)
    {
        Uint32 pix = SDL_MapRGB(image->format,100,250,250);
        put_pixel(image,i,0,pix);
    }
    for (int i = 0; i < w; i++)
    {
        Uint32 pix = SDL_MapRGB(image->format,100,250,250);
        put_pixel(image,i,h-1,pix);
    }
    for (int j = 0; j < h; j++)
    {
    Uint32 pix = SDL_MapRGB(image->format,100,250,250);
    put_pixel(image,0,j,pix);
    }
    for (int j = 0; j < h; j++)
    {
    Uint32 pix = SDL_MapRGB(image->format,100,250,250);
    put_pixel(image,w-1,j,pix);
    }
}

SDL_Surface *trim(SDL_Surface *image)
{
    size_t h = image->h;
    size_t w = image->w;
    size_t bottom_trim = 0;
    size_t top_trim = 0;
    size_t right_trim = 0;
    size_t left_trim = 0;

    for (size_t j = h-1; j > 0; j--)
    {
        for (size_t i = 0; i < w; i++)
        { //printf("bottom i -> %zu , j -> %zu\n",i,j);
            Uint32 pix = get_pixel(image, i, j);
            //If the pixel is not black
            if (pix != SDL_MapRGB(image->format, 255, 0, 0))
            {
                //trim here on the bottom
                bottom_trim = j - 1;
            }
        }
    }
    for (size_t j = 0; j < h; j++)
    {
        for (size_t i = 0; i < w; i++)
        { //printf("top i -> %zu , j -> %zu\n",i,j);
            Uint32 pix = get_pixel(image, i, j);
            //If the pixel is not black
            if (pix != SDL_MapRGB(image->format, 255, 0, 0))
            {
                //trim here on the top
                top_trim = j - 1;
            }
        }
    }
    for (size_t i = 0; i < w; i++)
    {
        for (size_t j = top_trim; j > bottom_trim; j--)
        { //printf("right i -> %zu , j -> %zu\n",i,j);
            Uint32 pix = get_pixel(image, i, j);
            //If the pixel is not black
            if (pix != SDL_MapRGB(image->format, 255, 0, 0))
            {
                //trim here on the right
                right_trim =  i - 1;
            }
        }
    }
    for (size_t i = w-1; i > 0; i--)
    {
        for (size_t j = top_trim; j > bottom_trim; j--)
        { //printf("left i -> %zu , j -> %zu\n",i,j);
            Uint32 pix = get_pixel(image, i, j);
            //If the pixel is not black
            if (pix != SDL_MapRGB(image->format, 255, 0, 0))
            {
                //trim here on the left
                left_trim =  i - 1;
            }
        }
    }
    SDL_Surface *output_image = SDL_CreateRGBSurface(0, (int)(w-(right_trim-left_trim)),(int)(h - (top_trim-bottom_trim)), 32, 0,0,0,0);
    if(output_image==NULL){

        printf("Failed to init -> %s\n",SDL_GetError());
        return image;
    }
    printf("new_width -> %d\n",(int)(w-(right_trim-left_trim)));
    printf("new_height -> %d\n", (int)(h - (top_trim-bottom_trim)));
    for (size_t i = left_trim; i < right_trim; i++)
    {
        for (size_t j = bottom_trim; j < top_trim; j++)
        {
            Uint32 pix = get_pixel(image, i, j);
            put_pixel(output_image,(int)(i-left_trim),(int)(j-bottom_trim),pix);
        }
    }
    SDL_FreeSurface(image);
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
    int *x_and_y = malloc(sizeof(int)*2);
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
    printf("angle -> %f\n",angle);
    angle = 3.14159265358979323846/2 - angle;
    
    double cosine = cos(angle);
    double sinus = sin(angle);
    int height = image->h;
    int width = image->w;
    int new_height = round(my_abs(height * cosine) + 
		    my_abs(width * sinus)) + 1;
    int new_width = round(my_abs(width * cosine) +
		    my_abs(height * sinus)) + 1;

    SDL_Surface *output_image = SDL_CreateRGBSurface(0,
		   2* new_width , 2*new_height , 32, 0, 0, 0, 0);
            change_to_red(output_image);

    int original_center_height = round(((height + 1) / 2) - 1);
    int original_center_width = round(((width + 1) / 2) - 1);

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
	        int *x_and_y = malloc(sizeof(int) * 2);
            int y = height - 1 - j - original_center_height;
            int x = width - 1 - i - original_center_width;
            x_and_y = new_sheer(angle, x, y);
            int new_x = x_and_y[0];
            int new_y = x_and_y[1];
            new_y = new_height - new_y;
            new_x = new_width - new_x;
	        free(x_and_y);
            Uint32 pixel = get_pixel(image, i, j);
            put_pixel(output_image, new_x, new_y, pixel);
        }
    }
    SDL_Surface *test = trim(output_image);
    change_red_to_white(test);
    test = rotation_270(test);
    return test;
}

