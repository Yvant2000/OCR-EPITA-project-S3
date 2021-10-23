#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "rotation.h"



int main(int argc, char *argv[])
{
    int quit = 0;

    if (argc < 3)
	    printf("Insufficient arguments\n");
    if(argv[1]==NULL){
        printf("Please input a path\n");
        return 1;
    }
    if(argv[2]==NULL){
        printf("Please input an angle\n");
        return 1;
    }
    SDL_Event event;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Sudoku",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 500, 500, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Surface *image = SDL_LoadBMP(argv[1]);
    if(image==NULL){
        printf("Your input path does not exist\n");
        return 1;
    }
    
    int num = str_to_int(argv[2]);
    printf("%d\n", num);
    image = rotate(image, num);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
    while (!quit)
    {
        SDL_WaitEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            quit = 1;
            break;
        }
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
