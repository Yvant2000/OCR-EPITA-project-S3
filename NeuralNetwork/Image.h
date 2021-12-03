//
// Created by yvesantoine on 30/11/2021.
//

#ifndef OCR_EPITA_PROJECT_S3_IMAGE_H
#define OCR_EPITA_PROJECT_S3_IMAGE_H

#include <SDL2/SDL.h>

SDL_Surface * load_image(char *path);
double * image_to_array(SDL_Surface * surface);

#endif //OCR_EPITA_PROJECT_S3_IMAGE_H
