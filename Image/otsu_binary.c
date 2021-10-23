#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include "basic_pixel.h"
#include <math.h>


float compute_first_order_cumulative_moment(float *hist, int k)
{
    float first_order_cumulative_moment = 0;
    for (int i = 0; i < k; i++) first_order_cumulative_moment += i*hist[i];
    return first_order_cumulative_moment;
}

float compute_zero_order_cumulative_moment(float *hist, int k)
{
    float zero_order_cumulative_moment = 0;
    for (int i = 0; i < k; i++) zero_order_cumulative_moment += hist[i];
    return zero_order_cumulative_moment;
}

float compute_variance_class_separability(float uT,float wk, float uk)
{
    return ((uT*wk-uk)*(uT*wk-uk))/(wk*(1-wk));
}

int Otsu(SDL_Surface* image_surface)
{

    int width = image_surface -> w;
    int height = image_surface -> h;

    int all_pixels = width*height;
   
    float histogram[256];

    for (size_t i = 0; i < 256; i++)
    {
       histogram[i] = 0;
    }

    for(int w=0;w<width;w++)
    {
        for(int h=0;h<height;h++)
        {
            Uint32 pixel = get_pixel(image_surface,w,h);
            Uint8 r,g,b = 0;
            SDL_GetRGB(pixel, image_surface->format, &r,&g,&b);

            histogram[(r+g+b)/3] += 1;
        }
    }

    for(int i =0;i<256;i++) histogram[i] /= all_pixels;

    float w[256],u[256],uT;

    for (int i = 0; i < 256; i++)

    {

            w[i] = compute_zero_order_cumulative_moment(histogram, i);

            u[i] = compute_first_order_cumulative_moment(histogram, i);

    }

    uT = compute_first_order_cumulative_moment(histogram, 256);

    float variance_class_separability_max = -1;

    float best_threesold = 0;

    for (int i = 0; i < 256; i++)

    {

        int vk = compute_variance_class_separability(uT, w[i], u[i]);

        if (vk > variance_class_separability_max)
        {
            variance_class_separability_max = vk;
            best_threesold = i;
        }

    }

    /*for (int w = 0; w < width; w++)
    {
        for (int h = 0; h < height; h++)
        {
            Uint32 pixel = get_pixel(image_surface,w,h);
            Uint8 r,g,b = 0;
            SDL_GetRGB(pixel, image_surface->format, &r,&g,&b);

            histogram[(r+g+b)/3]++;
        }
    }
    */

    return best_threesold;
}

void OtsuBinarization(SDL_Surface* image, int threshold)
{
    int width = image -> w;
    int height = image -> h;
    for (int i = 0; i < width; i++)
    {
        for (int j = 0 ; j < height; j++)
        {
            Uint32 pixel = get_pixel(image,i,j);
            Uint8 r,g,b = 0;
            SDL_GetRGB(pixel, image->format, &r,&g,&b);
            if ((r+g+b)/3 > threshold)
            {
                Uint32 black = SDL_MapRGB(image->format, 255, 255, 255);
                put_pixel(image,i,j,black);
            }
            else
            {
                Uint32 white = SDL_MapRGB(image->format, 0, 0, 0);
                put_pixel(image,i,j,white);
            }
        }
    }
}

float compute_first_order_cumulative_moment1(float *hist, int k)

{

    float first_order_cumulative_moment = 0;

    for (int i = 0; i < k; i++)

    {

            first_order_cumulative_moment += i*hist[i];
            if(k == 256 && i == 2)
                printf("res = %f n = %f", first_order_cumulative_moment, hist[i]);
    }


    return first_order_cumulative_moment;

}


float compute_zero_order_cumulative_moment1(float *hist, int k)

{

    float first_order_cumulative_moment = 0;

    for (int i = 0; i < k; i++)

    {

            first_order_cumulative_moment += hist[i];

    }


    return first_order_cumulative_moment;

}

float compute_variance_class_separability1(float uT,float wk, float uk)

{

    return pow((uT*wk-uk),2)/(wk*(1-wk));

}

void Otsu1(SDL_Surface* image)
{
    int width = image->w;

    int height = image->h;

    int T = height * width;

    printf(" T = %i \n", T);

    float hist[256];

    for (int i = 0; i < 256; i++)

    {

        hist[i] = 0;

    }

    for (int i = 0; i < width; i++)
    {
        for (int k = 0; k < height; k++)
        {
            Uint32 pixel = get_pixel(image, i, k);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            hist[(r+g+b)/3] += 1;
        }        
    }

    for (int i = 0; i < 256; i++)
    {
        if (i == 2)
            printf("res = %f T = %i \n", hist[i], T);
        hist[i] = hist[i]/T;

    }

    float w[256],u[256],uT;

    for (int i = 0; i < 256; i++)

    {
        w[i] = compute_zero_order_cumulative_moment1(hist, i);
        u[i] = compute_first_order_cumulative_moment1(hist, i);
    }

    uT = compute_first_order_cumulative_moment1(hist, 256);
    printf("uT = %f\n", uT);


    float variance_class_separability_max = 100;

    float best_threesold = 0;

    for (int i = 0; i < 256; i++)

    {
            int vk = compute_variance_class_separability1(uT, w[i], u[i]);


            if (vk > variance_class_separability_max)

            {

                    variance_class_separability_max = vk;

                    best_threesold = i;

            }

    }

    printf("best = %f", best_threesold);

    for (int i = 0; i < width; i++)

    {
            for (int k = 0; k < height; k++)
            {
                Uint32 pixel = get_pixel(image, i, k);
                Uint8 r, g, b;
                SDL_GetRGB(pixel, image->format, &r, &g, &b);
                if ((r + g + b) / 3 < best_threesold - (abs((int)(130-best_threesold))*3))
                {
                    Uint32 new_pixel = SDL_MapRGB(image->format, 0, 0, 0);
                    put_pixel(image, i, k, new_pixel);
                }

                else
                {
                    Uint32 new_pixel = SDL_MapRGB(image->format, 255, 255, 255);
                    put_pixel(image, i, k, new_pixel);
                }
            }

    }





}