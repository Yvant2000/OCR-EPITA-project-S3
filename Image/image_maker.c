#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <math.h>
#include <err.h>
#include "basic_pixel.h"
#include "sudoku_res.h"
#include "primary_filters.h"

static char numbers[9][25] =
{
    "Image/grid_numbers/1.bmp",
    "Image/grid_numbers/2.bmp",
    "Image/grid_numbers/3.bmp",
    "Image/grid_numbers/4.bmp",
    "Image/grid_numbers/5.bmp",
    "Image/grid_numbers/6.bmp",
    "Image/grid_numbers/7.bmp",
    "Image/grid_numbers/8.bmp",
    "Image/grid_numbers/9.bmp",
};

void fill_surface(SDL_Surface *number,SDL_Surface *grid, int x_start, int y_start)
{
    for (int i = x_start; i < x_start+49; i++)
    {
        for (int j = y_start; j < y_start+49; j++)
        {
            //printf("i -> %d j -> %d\n",i-x_start,j-y_start);
            Uint32 pix = get_pixel(number,i-x_start,j-y_start);
            put_pixel(grid,i,j,pix);
        }
    }
}

void blue(SDL_Surface *grid)
{
    for (int i = 0; i < grid->w; i++)
    {
        for (int j = 0; j < grid->h; j++)
        {
            //printf("i -> %d j -> %d\n",i-x_start,j-y_start);
            Uint32 pix = get_pixel(grid,i,j);
            if(pix == SDL_MapRGB(grid->format,0,0,255))
            {
                Uint32 pix = SDL_MapRGB(grid->format,255,255,255);
                put_pixel(grid,i,j,pix);
            }
        }
    }
}

void create_image(char *path,char *save)
{
    SDL_Surface *grid = SDL_CreateRGBSurface(0, 461, 461, 32, 0, 0, 0, 0);
    int *matrix = malloc(sizeof(int) * 81);
    matrix = parse_file(path);
    int x_start = 2;
    int y_start = 2;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            int num = matrix[(i * 9) + j];
            SDL_Surface *number = SDL_LoadBMP(numbers[num-1]);
            if (number == NULL) {
                printf("can't lod %s\n", numbers[num - 1]);
                exit(1);
            }
            fill_surface(number,grid,x_start,y_start);
            x_start += 51;
            SDL_FreeSurface(number);
        }
        y_start += 51;
        x_start = 2;
    }
    free(matrix);
    blue(grid);
    grey_scale(grid);
    SDL_SaveBMP(grid,save);
}

