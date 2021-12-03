//
// Created by yvesantoine on 30/11/2021.
//

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>

SDL_Surface *  load_image(char *path)
{
    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    SDL_Surface * img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());
    return img;
}

static inline Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch (surface->format->BytesPerPixel)
    {
        case 1: return *p;
        case 2: return *(Uint16 *)p;
        case 3: return p[0] | p[1] << 8 | p[2] << 16;
        case 4: return *(Uint32 *)p;
    }
    return 0;
}
double * image_to_array(SDL_Surface * surface)
{
    /* Expect a valid input image of size 28 * 28 */

    double * array = malloc(sizeof (double) * 784);
    for (unsigned x = 0; x < 28; x++){
        for(unsigned y = 0; y < 28; y++){
            Uint32 pixel = get_pixel(surface, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, surface->format, &r, &g, &b);
            // printf("%d %d %d\n", r, g, b);
            array[x * 28 + y] = (255. - ((double)(r + g + b)) / 3) / 255.;
        }
    }
    SDL_FreeSurface(surface);
    //for (int i = 0;i < 784;i++)printf("%lf   ", array[i]); printf("\n");
    return array;
}