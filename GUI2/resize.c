#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "resize.h"

#include <math.h>

Uint8* pixel_ref_bis(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

void set_pixel_bis(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
      Uint8 *target_pixel = (Uint8 *)surface->pixels + y * surface->pitch + x * 4;
      *(Uint32 *)target_pixel = pixel;
}

Uint32 get_pixel_bis(SDL_Surface *surface, unsigned x, unsigned y)
{
    Uint8 *p = pixel_ref_bis(surface, x, y);

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


SDL_Surface *rescale(SDL_Surface *picture, int newwidth, int newheight)
{
    Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN    //post-parsing
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    SDL_Surface *newpic = SDL_CreateRGBSurface(0, newwidth, newheight, 32, rmask, gmask, bmask, amask);
    //SDL_Rect srcrect, dstrect;
    //srcrect.x = 0;
    //srcrect.y = 0;
    //srcrect.w = picture->w;
    //srcrect.h = picture->h;
    //dstrect.x = 0;
    //dstrect.y = 0;
    //dstrect.w = newwidth;
    //dstrect.h = newheight;
    for(int i = 0; i < newwidth; i++)
    {
        for(int j = 0; j < newheight; j++)
        {
            int x = (int)((double)i / (double)newwidth * (double)picture->w);
            int y = (int)((double)j / (double)newheight * (double)picture->h);
            set_pixel_bis(newpic, i, j, get_pixel_bis(picture, x, y));
        }
    }
    SDL_FreeSurface(picture);
    return newpic;
}

/*
int main(int argc, char *argv[])
{
    if (argc != 5) 
        return 1;
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    SDL_Surface *picture = SDL_LoadBMP(argv[1]);
    if(!picture) {
        printf("IMG_Load: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Surface *resized = rescale(picture, atoi(argv[2]), atoi(argv[3]));
    SDL_SaveBMP(resized, argv[4]);
    SDL_FreeSurface(picture);
    SDL_FreeSurface(resized);
    return 0;
}*/
