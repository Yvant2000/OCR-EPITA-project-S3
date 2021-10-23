#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>

#ifndef FINAL_DETECTION_H
#define FINAL_DETECTION_H

void detect_green(SDL_Surface *image_green, SDL_Surface *original,char *str_path);
SDL_Surface *clean_up(SDL_Surface *image);

#endif