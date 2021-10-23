#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


#ifndef SOBEL_H
#define SOBEL_H

SDL_Surface *horizontal_edge_detection_sobel(SDL_Surface *image);
SDL_Surface *horizontal_edge_detection_sobel1(SDL_Surface *image);
SDL_Surface *vertical_edge_detection_sobel(SDL_Surface *image);
SDL_Surface *vertical_edge_detection_sobel1(SDL_Surface *image);
SDL_Surface *hysteris(SDL_Surface *image);
SDL_Surface *combine_sobel(SDL_Surface *image_hor, SDL_Surface *image_vert);

#endif
