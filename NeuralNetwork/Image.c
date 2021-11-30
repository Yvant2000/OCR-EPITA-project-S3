//
// Created by yvesantoine on 30/11/2021.
//

#include <SDL2/SDL.h>
#include <SDL2/SDL2_Image.h>
#include <err.h>

SDL_Surface * load_image(char *path)
{
    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    SDL_Surface * img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}

float * image_to_array(SDL_Surface * surface)
{
    /* Expect a valid input image of size 28 * 28 */

    float * array = malloc(sizeof (float) * 784);

    return array;
}