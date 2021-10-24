#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include "basic_pixel.h"

typedef struct array_type
{
    size_t len;
    int *array_n;
} array_type;

SDL_Surface *detect_motive_hor(SDL_Surface *image)
{
    int h = image->h;
    int w = image->w;
    size_t array_size = h / 5;
    array_type *ar = malloc(sizeof(array_type));
    ar->len = array_size;
    ar->array_n = malloc(sizeof(int) * array_size);
    SDL_Surface *output = SDL_CreateRGBSurface(0,w,h,32,0,0,0,0);
    for(int i =0; i< w; i++){
      for(int j=0; j < h; j++){
        Uint32 pixel = get_pixel(image, i, j);
        put_pixel(output,i,j,pixel);
       }
    }
    for (size_t i = 0; i < array_size; i++)
    {
        ar->array_n[i] = 0;
    }
    for (int i = 0; i < w; i++)
    {
        int j = 0;
        int serie = 0;
        Uint32 pixel = get_pixel(image, i, j);

        //If the pixel seen  not is red
        while (pixel != SDL_MapRGB(image->format, 255, 0, 0) && j < h)
        {
            
            pixel = get_pixel(image, i, j);
	    j+=1;
        }
        while (j < h)
        {
            pixel = get_pixel(image, i, j);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            //scan untill the red ends
            while (pixel == SDL_MapRGB(image->format, 255, 0, 0) && j < image->h) //while the pixels are red in other words not black
            {
                
                pixel = get_pixel(image, i, j);
		j+=1;
            }
            while (pixel != SDL_MapRGB(image->format, 255, 0, 0) && j < image->h) //while the pixels are black in other words not red
            {
                serie += 1;
                
                pixel = get_pixel(image, i, j);
		j+=1;
                if (serie >= h / 5)
                {
                    serie = 0;
                    break;
                }
            }
            //We saw something else than black
            if (serie > 1)
            {
                ar->array_n[serie - 1] += 1;
                serie = 0;
            }
            if (j >= image->h)
            {
                break;
            }
        }
    }
    int max_occ = 0;
    size_t max_index = 0;
    for (size_t i = 0; i < array_size; i++)
    {
        int val = ar->array_n[i];
        if (val > 100 && i > 28)
        {
            if (max_occ < val)
            {
                max_occ = val;
                max_index = i;
            }
        }
    }
    for (int i = 0; i < w; i++)
    {
        int j = 0;
        int serie = 0;
        Uint32 pixel = get_pixel(image, i, j);
        //If the pixel seen  not is red
        while (pixel != SDL_MapRGB(image->format, 255, 0, 0) && j < image->h)
        {
            
            pixel = get_pixel(image, i, j);
	    j+=1;
        }
        while (j < h)
        {
            pixel = get_pixel(image, i, j);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            //scan untill the red ends
            while (pixel == SDL_MapRGB(image->format, 255, 0, 0) && j < image->h) //while the pixels are red in other words not black
            {
                
                pixel = get_pixel(image, i, j);
		j+=1;
            }
            while (pixel != SDL_MapRGB(image->format, 255, 0, 0) && j < image->h) //while the pixels are black in other words not red
            {
                serie += 1;
                
                pixel = get_pixel(image, i, j);
		j+=1;
                if (serie >= h / 5)
                {
                    serie = 0;
                    break;
                }
            }
            if (serie >= (int)max_index - (h / 95) && serie <= (int)max_index + (h / 95))
            {
                int actual_j = j - serie;
                while (j != actual_j)
                {

                    Uint32 pixel = SDL_MapRGB(image->format, 100, 0, 100);
                    put_pixel(output, i, actual_j, pixel);
                    actual_j += 1;
                }
            }
            //We saw something else than black
            if (serie > 1)
            {
                ar->array_n[serie - 1] += 1;
                serie = 0;
            }
            if (j >= image->h)
            {
                break;
            }
        }
    }
    free(ar);
    return output;
}

SDL_Surface *detect_motive_vert(SDL_Surface *image)
{
    int h = image->h;
    int w = image->w;
    size_t array_size = w / 5;
    array_type *ar = malloc(sizeof(array_type));
    ar->len = array_size;
    ar->array_n = malloc(sizeof(int) * array_size);
    SDL_Surface *output = SDL_CreateRGBSurface(0,w,h,32,0,0,0,0);
    for(int i =0; i< w; i++){
      for(int j=0; j < h; j++){
        Uint32 pixel = get_pixel(image, i, j);
        put_pixel(output,i,j,pixel);
       }
     }
    for (size_t i = 0; i < array_size; i++)
    {
        ar->array_n[i] = 0;
    }
    for (int i = 0; i < h; i++)
    {
        int j = 0;
        int serie = 0;
        Uint32 pixel = get_pixel(image, j, i);
        //If the pixel seen  not is red
        while (pixel != SDL_MapRGB(image->format, 255, 0, 0) && j < w)
        {
            
            pixel = get_pixel(image, j, i);
	    j+=1;
        }
        while (j < w)
        {
            pixel = get_pixel(image, j, i);

            while (pixel == SDL_MapRGB(image->format, 255, 0, 0) && j < w) //while the pixels are red in other words not black
            {
                
                pixel = get_pixel(image, j, i);
		j+=1;
            }
            while (pixel != SDL_MapRGB(image->format, 255, 0, 0) && j < w) //while the pixels are black in other words not red
            {
                serie += 1;
                pixel = get_pixel(image, j, i);
		j+=1;
                if (serie >= w / 5)
                {
                    serie = 0;
                    break;
                }
            }
            //We saw something else than black
            if (serie > 1)
            {
                ar->array_n[serie - 1] += 1;
                serie = 0;
            }
            if (j >= w)
            {
                break;
            }
        }
    }
    printf("Out of the first loop\n");
    int max_occ = 0;
    size_t max_index = 0;
    for (size_t i = 0; i < array_size; i++)
    {
        int val = ar->array_n[i];
        if (val > 100 && i > 28)
        {
            if (max_occ < val)
            {
                max_occ = val;
                max_index = i;
            }
        }
    }
    printf("After max-occ\n");
    for (int i = 0; i < h; i++)
    {
        int j = 0;
        int serie = 0;
        Uint32 pixel = get_pixel(image, j, i);
        //If the pixel seen  not is red
        while (pixel != SDL_MapRGB(image->format, 255, 0, 0) && j < w)
        {
	printf("Not read: i -> %d , j -> %d\n",i,j);
            pixel = get_pixel(image, j, i);
	    j+=1;
        }
        while (j < w)
        {
		printf("Read: i -> %d , j -> %d\n",i,j);
            pixel = get_pixel(image, j, i);
            //scan untill the red ends
            while (pixel == SDL_MapRGB(image->format, 255, 0, 0) && j < w) {
	//while the pixels are red in other words not black
	     printf("Read: i -> %d , j -> %d\n",i,j);   
             pixel = get_pixel(image, j, i);
	    j+=1;
	    }
            while (pixel != SDL_MapRGB(image->format, 255, 0, 0) && j < w) //while the pixels are black in other words not red
            {
		serie+=1;
                pixel = get_pixel(image, j, i);
		j+=1;
                if (serie >= w / 5)
                {
                    serie = 0;
                    break;
                }
            }

            if (serie >= (int)max_index - (w / 95) && serie <= (int)max_index + (w / 95))
            {
                int actual_j = j - serie;
                while (j != actual_j){
		  
		      put_pixel(output, actual_j, i, SDL_MapRGB(image->format, 0, 255, 0));
		  
		   actual_j+=1;
		}
            }
            //We saw something else than black
            if (serie > 1)
            {
                ar->array_n[serie - 1]+=1;
                serie = 0;
            }
            if (j >= w){
                break;
	    }
        }
    }
    printf("All Good\n");
    free(ar);
    return output;
}

SDL_Surface * combine_detections(SDL_Surface *image, SDL_Surface *image1)
{ 
int h = image->h;
int w = image->w;
SDL_Surface *final_image = SDL_CreateRGBSurface(0,w,h,32,0,0,0,0);
    for (int i = 0; i < image->w; i++){
	    for (int j = 0; j < image->h; j++){
             if (get_pixel(image, i, j) == SDL_MapRGB(image->format, 100, 0, 100)){
		if (get_pixel(image1, i, j) == SDL_MapRGB(image1->format, 0, 255, 0)){
	            put_pixel(final_image, i, j, SDL_MapRGB(image->format, 100, 255, 100));
		}

        }
	     else{
		     put_pixel(final_image,i,j,get_pixel(image, i, j));
	     }

      }
   }
    return final_image;
}
