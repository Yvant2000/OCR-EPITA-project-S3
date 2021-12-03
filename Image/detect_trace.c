#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "basic_pixel.h"


void detect_beg_end_incolumn(SDL_Surface *image, int division, int borders[2], int column, int significant)
{
    int global = 1;
    int h = image->h;
    int black = 1; //bool
    int white = 1; //bool
    //index of the beggining of a pattern
    int first_cordinate = 0;
    //index of the end of a pattern
    int last_cordinate = 0;
    // Run throught the whole line
    while (global < h)
    {
        //Scan throught black
        if (black)
        {
            for (int i = 0; global < h && (pixel_grey(image, column, global) < division); i++)
            {
                global += 1;
            }
            // Until you find something white
            white = 1;
            black = 0;
        }
        //We know we found something white
        if (white)
        {
            //We are going to count the number of occurencies of white
            int white_occurence = 0;
            for (int i = 0; global < h && (pixel_grey(image, column, global) >= division); i++)
            {
                //INcrement the index to look at the next pixel
                global += 1;
                //increment the white occurence
                white_occurence += 1;
            }
            //If the occurence of white can be considered as significant for this image
            if (white_occurence > significant)
            {
                //If it is the first white patter we encounter
                if (first_cordinate == 0)
                {
                    first_cordinate = global - white_occurence;
                    last_cordinate = global;
                }
                //Otherwise just move the last-point pointer
                else
                {
                    last_cordinate = global;
                }
            }

            white = 0;
            black = 1;
        }
    }
    borders[0] = first_cordinate;
    borders[1] = last_cordinate;
}
//Exactly the same algorithm as the column detection but applied to a vertical line

void detect_beg_end_inline(SDL_Surface *image,  struct list *borders, int line)
{
    int global = 1;
    int w = image->w;
    int black = 1; //bool
    int white = 1; //bool
    int first_cordinate = 0;
    int last_cordinate = 0;
    while (global < w)
    {
        if (black)
        {
            for (int i = 0; global < w && (pixel_grey(image, global, line) == 0); i++)
            {
                global += 1;
            }
            white = 1;
            black = 0;
        }
        if (white)
        {
            int white_occurence = 0;
            for (int i = 0; global < w && (pixel_grey(image, global, line) == 255); i++)
            {
                global += 1;
                white_occurence += 1;
            }

            if (white_occurence > w * 0.1)
            {
                if (first_cordinate == 0)
                {
                    first_cordinate = global - white_occurence;
                    last_cordinate = global;
                }
                else
                {
                    last_cordinate = global;
                }

                white = 0;
                black = 1;
            }

            else
            {
                white = 0;
                black = 1;
            }
        }
    }
}

void trace_beg_end_line(SDL_Surface *image)
{
    int *borders = NULL;
    for (int i = 0; i < image->h; i++)
    {
        detect_beg_end_inline(image, borders, i);
        for (int j = borders[0] + 1; j < borders[1]; j++)
        {
            Uint32 pixel1_new = SDL_MapRGB(image->format, 255, 2, 2);
            put_pixel(image, j, i, pixel1_new);
        }
    }
}

void trace_beg_end_column(SDL_Surface *image, int division, int significant)
{
    int borders[2] = {0, 0};
    for (int i = 0; i < image->w; i++)
    {
        detect_beg_end_incolumn(image, division, borders, i, significant);
        for (int j = borders[0] + 1; j < borders[1]; j++)
        {
            Uint32 pixel1_new = SDL_MapRGB(image->format, 255, 2, 2);
            put_pixel(image, i, j, pixel1_new);
        }
    }
}
