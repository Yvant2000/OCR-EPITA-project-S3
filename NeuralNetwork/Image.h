//
// Created by yvesantoine on 30/11/2021.
//

#ifndef OCR_EPITA_PROJECT_S3_IMAGE_H
#define OCR_EPITA_PROJECT_S3_IMAGE_H

#include <SDL2/SDL.h>

SDL_Surface * load_image(char *path);
float * image_to_array(SDL_Surface * surface);
void image_init();
void image_quit();

#endif //OCR_EPITA_PROJECT_S3_IMAGE_H
