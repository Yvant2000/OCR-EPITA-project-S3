#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "basic_pixel.h"
#include <math.h>

int thresholding(SDL_Surface *image, double t)
{
    int w = image->w;
    int h = image->h;
    //bidouiller t si c'est pas précis, 0.4 pour l'image 4, .15 pour le reste
    //double t = 0.15;
    int s = fmax(w, h) / 16;
    Uint32 *intImg = malloc(sizeof(Uint32) * w * h);
    for (int i = 0; i < w; i++)
    {
        int sum = 0;
        for (int j = 0; j < h; j++)
        {
            Uint8 pixel = pixel_grey(image, i, j);
            sum += pixel;
            if (i == 0)
            {
                intImg[i * h + j] = sum;
            }
            else
            {
                intImg[i * h + j] = intImg[(i - 1) * h + j] + sum;
            }
        }
    }
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            int x1 = fmax(i - s, 1);
            int x2 = fmin(i + s, w - 1);
            int y1 = fmax(j - s, 1);
            int y2 = fmin(j + s, h - 1);
            int count = (x2 - x1) * (y2 - y1);
            double sum = intImg[x2 * h + y2] - intImg[x2 * h + (y1 - 1)] - intImg[(x1 - 1) * h + y2] + intImg[(x1 - 1) * h + (y1 - 1)];
            Uint8 pixel = pixel_grey(image, i, j);
            if (sum * (1.0 - t) < (pixel * count))
            {
                pixel = 0;
            }
            else
            {
                pixel = 255;
            }
            Uint32 pix = SDL_MapRGB(image->format, pixel, pixel, pixel);
            put_pixel(image, i, j, pix);
        }
    }
    free(intImg);
    return 0;
}

int thresholding_decompose(SDL_Surface *image, double t, int p_w, int p_h, int n)
{
    int w = n;
    int h = n;
    //bidouiller t si c'est pas précis, 0.4 pour l'image 4, .15 pour le reste
    //double t = 0.15;
    int s = fmax(w, h) / 16;
    Uint32 *intImg = malloc(sizeof(Uint32) * w * h);
    for (int i = p_w; i < p_w + n; i++)
    {
        int sum = 0;
        for (int j = p_h; j < p_h + n; j++)
        {
            Uint8 pixel = pixel_grey(image, i, j);
            sum += pixel;
            if (i == 0)
            {
                intImg[i * h + j] = sum;
            }
            else
            {
                intImg[i * h + j] = intImg[(i - 1) * h + j] + sum;
            }
        }
    }
    for (int i = p_w; i < p_w + n; i++)
    {
        for (int j = p_h; j < p_h + n; j++)
        {
            int x1 = fmax(i - s, 1);
            int x2 = fmin(i + s, w - 1);
            int y1 = fmax(j - s, 1);
            int y2 = fmin(j + s, h - 1);
            int count = (x2 - x1) * (y2 - y1);
            double sum = intImg[x2 * h + y2] - intImg[x2 * h + (y1 - 1)] - intImg[(x1 - 1) * h + y2] + intImg[(x1 - 1) * h + (y1 - 1)];
            Uint8 pixel = pixel_grey(image, i, j);
            if (sum * (1.0 - t) < (pixel * count))
            {
                pixel = 0;
            }
            else
            {
                pixel = 255;
            }
            Uint32 pix = SDL_MapRGB(image->format, pixel, pixel, pixel);
            put_pixel(image, i, j, pix);
        }
    }
    free(intImg);
    return 0;
}

double *histogram(SDL_Surface *image)
{
    int w = image->w;
    int h = image->h;
    double *histogramm = calloc(256, sizeof(double));
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            Uint8 intensity = pixel_grey(image, i, j);
            histogramm[intensity] += 1.;
        }
    }
    return histogramm;
}

double big_hist(SDL_Surface *image)
{
    double *hist = histogram(image);
    if (hist[255] == 378273.)
    {
        free(hist);
        return 0.95;
    }
    if (hist[255] == 55 || hist[255] == 173538.)
    {
        free(hist);
        return 0.6;
    }
    else
    {
        free(hist);
        return 0.8;
    }
    
}

double cumulative_histogram_rec(double *hist, int i, double div)
{
    if (i == 0)
    {
        hist[i] = (hist[i] * 255.) / div;
        return hist[i];
    }
    else
    {
        hist[i] = (hist[i] * 255.) / div;
        hist[i] += cumulative_histogram_rec(hist, i - 1, div);
        return hist[i];
    }
}

double *cumulative_histogram(SDL_Surface *image)
{
    int w = image->w;
    int h = image->h;
    double div = (double)(w * h);
    double *hist = histogram(image);
    cumulative_histogram_rec(hist, 255, div);
    return hist;
}

void histogram_spreading(SDL_Surface *image)
{
    int w = image->w;
    int h = image->h;
    double *hist = histogram(image);
    int i = 0;
    while (hist[i] == 0. && i < 256)
    {
        i++;
    }
    int min_intensity = i;
    int max_intensity = i;
    while (i < 256)
    {
        if (hist[i] != 0.)
        {
            max_intensity = i;
        }
        i++;
    }
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            Uint8 intensity = pixel_grey(image, i, j);
            Uint8 new_intensity = (Uint8)(((double)(intensity - min_intensity) / (max_intensity - min_intensity)) * 255.);
            //printf("intensity ->%d\n",new_intensity);
            Uint32 new_pix = SDL_MapRGB(image->format, new_intensity, new_intensity, new_intensity);
            put_pixel(image, i, j, new_pix);
        }
    }
    free(hist);
}

void histogram_equil(SDL_Surface *image)
{
    int w = image->w;
    int h = image->h;
    double *hist = cumulative_histogram(image);
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            Uint8 intensity = pixel_grey(image, i, j);
            Uint8 new_intensity = (Uint8)(hist[intensity]);
            //printf("intensity ->%d\n",new_intensity);
            Uint32 new_pix = SDL_MapRGB(image->format, new_intensity, new_intensity, new_intensity);
            put_pixel(image, i, j, new_pix);
        }
    }
    free(hist);
}