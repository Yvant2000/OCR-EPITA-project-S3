#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef INTERFACE_H
#define INTERFACE_H

void Display(char *path);
void Load();
void Save();
void Rotation();
void About();
void ResizeImage(SDL_Surface *image);
int main_gui();

#endif
