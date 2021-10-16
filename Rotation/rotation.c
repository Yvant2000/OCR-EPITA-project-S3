#include <stdio.h>
#include <math.h>
#include "string.h"
#include <SDL.h>
#include <SDL_image.h>

static inline Uint8 *pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8 *)surf->pixels + y * surf->pitch + x * bpp;
}

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch (surface->format->BytesPerPixel)
    {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;
    }

    return 0;
}

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch (surface->format->BytesPerPixel)
    {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
        {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        }
        else
        {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

int *new_sheer(double angle, int x, int y)
{
    int *x_and_y = malloc(sizeof(int) * 2);
    //sheer 1
    double intan = angle / 2.f;
    // printf("intan -> %.6f\n",intan);
    double tangent = tan(intan);
    // printf("tangent for angel -> %f: is -> %.6f\n",angle,tangent);
    int new_x = round(x - y * tangent);
    // printf("new_x -> %d \n",new_x);
    int new_y = y;
    // printf("new_y -> %d \n",new_y);
    //sheer 2
    new_y = round(new_x * sin(angle) + new_y);
    // printf("new_y -> %d \n",new_y);
    //sheer 3
    new_x = round(new_x - new_y * tangent);
    // printf("new_x -> %d \n",new_x);
    x_and_y[0] = new_x;
    x_and_y[1] = new_y;
    return x_and_y;
}
SDL_Surface *rotate(SDL_Surface *image, double angle)
{
    int *x_and_y = malloc(sizeof(int) * 2);
    angle = angle / 57.29578;
    // printf("angle %f\n", angle);
    double cosine = cos(angle);
    // printf("cosine %f\n", cosine);
    double sinus = sin(angle);
    // printf("sinus %f\n", sinus);
    int height = image->h;
    int width = image->w;
    int new_height = round((abs(height * cosine) + abs(width * sinus))) + 1;
    printf("%d new_height\n", new_height);
    int new_width = round((abs(width * cosine) + abs(height * sinus))) + 1;
    printf("%d new_width\n", new_width);
    SDL_Surface *output_image = SDL_CreateRGBSurface(0, new_width + 100, new_height + 100, 32, 0, 0, 0, 0);

    int original_center_height = round((double)(((height + 1) / 2) - 1));
    printf("original_centre_height ->  %zu :\n", original_center_height);
    int original_center_width = round((double)(((width + 1) / 2) - 1));
    printf("original_centre_width ->  %zu :\n", original_center_width);
    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            int y = height - 1 - i - original_center_height;
            int x = width - 1 - j - original_center_width;
            // printf("y -> %d : x -> %d : \n",y,x);
            x_and_y = new_sheer(angle, x, y);
            int new_x = x_and_y[0];
            int new_y = x_and_y[1];
            // printf("new_y -> %d : new_x -> %d : \n",new_x,new_y);
            new_y = new_height - new_y;
            new_x = new_width - new_x;
            // printf("new_y -> %d : new_x -> %d : after normalisation\n",new_y,new_x);
            Uint32 pixel = get_pixel(image, i, j);
            put_pixel(output_image, new_y, new_x, pixel);
        }
    }
    free(x_and_y);
    return output_image;
}

int str_to_int(char *num)
{
    size_t i = 0;
    int number = 0;
    while (num[i] != 0)
    {
        number *= 10;
        int digit = num[i] - '0';
        printf("%d\n",digit);
        number += digit;
        i+=1;
    }
    return number;
}
