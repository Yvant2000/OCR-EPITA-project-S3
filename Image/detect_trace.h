#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef DETECT_TRACE_H
#define DETECT_TRACE_H

void trace_beg_end_line(SDL_Surface *image);
void trace_beg_end_column(SDL_Surface *image, int percentage, int division, int significant);

#endif