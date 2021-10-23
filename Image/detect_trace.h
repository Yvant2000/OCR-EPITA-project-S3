#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>

#ifndef DETECT_TRACE_H
#define DETECT_TRACE_H

void trace_beg_end_line(SDL_Surface *image, int percentage, int division, int significant);
void trace_beg_end_column(SDL_Surface *image, int percentage, int division, int significant);

#endif