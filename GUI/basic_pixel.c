#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

//Gives a pointer reference to a fixel
static inline Uint8 *pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8 *)surf->pixels + y * surf->pitch + x * bpp;
}

void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
      Uint8 *target_pixel = (Uint8 *)surface->pixels + y * surface->pitch + x * 4;
      *(Uint32 *)target_pixel = pixel;
}

// Gets a physical Uint32 pixel from SDL_Surface image
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
// Puts a Uint32 pixel into SDL_Surface
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

//The function return the greyscale value of a pixel
//This function applies only to a grayscaled image
Uint8 pixel_grey(SDL_Surface *image, unsigned int i, unsigned int j)
{
    Uint32 pixel = get_pixel(image, i, j);
    Uint8 r, g, b;
    SDL_GetRGB(pixel, image->format, &r, &g, &b);
    return r;
}
