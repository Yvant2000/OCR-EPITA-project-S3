#ifndef RESIZE_H
#define RESIZE_H

Uint8* pixel_ref_bis(SDL_Surface *surf, unsigned x, unsigned y);
void set_pixel_bis(SDL_Surface *surface, int x, int y, Uint32 Pixel);
Uint32 get_pixel_bis(SDL_Surface *surface, unsigned x, unsigned y);
SDL_Surface *rescale(SDL_Surface *picture, int newwidth, int newheight);

#endif
