
//gcc src/*.c -o bin/prog -I include -L lib -lmingw32 -lSDL2main -lSDL2

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <math.h>
#include "string.h"

typedef struct array_type
{
    size_t len;
    int *array_n;
} array_type;

SDL_Surface *any(SDL_Surface *image)
{
    int h = image->h;
    int w = image->w;
    SDL_Surface *image_coppy = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    for (size_t i = 0; i < w; i++)
    {
        for (size_t j = 0; j < h; j++)
        {
        }
    }
}

static inline Uint8 *pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8 *)surf->pixels + y * surf->pitch + x * bpp;
}

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch (surface->format->BytesPerPixel)
    {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;
    }

    return 0;
}

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch (surface->format->BytesPerPixel)
    {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
        {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        }
        else
        {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

//The function return the greyscale value of a pixel
//This function applies only to a grayscaled image
Uint8 pixel_grey(SDL_Surface *image, unsigned int i, unsigned int j)
{
    Uint32 pixel = get_pixel(image, i, j);
    Uint8 r, g, b;
    SDL_GetRGB(pixel, image->format, &r, &g, &b);
    return r;
}

Uint8 kernel5x5(float a0,
                float a1,
                float a2,
                float a3,
                float a4,
                float b0,
                float b1,
                float b2,
                float b3,
                float b4,
                float c0,
                float c1,
                float c2,
                float c3,
                float c4,
                float d0,
                float d1,
                float d2,
                float d3,
                float d4,
                float e0,
                float e1,
                float e2,
                float e3,
                float e4,
                SDL_Surface *image, unsigned int i, unsigned int j)
{
    return ((a0)*pixel_grey(image, i - 2, j - 2) + (a1)*pixel_grey(image, i - 1, j - 2) + (a2)*pixel_grey(image, i, j - 2) + (a3)*pixel_grey(image, i + 1, j - 2) + (a4)*pixel_grey(image, i + 2, j - 2) +
            (b0)*pixel_grey(image, i - 2, j - 1) + (b1)*pixel_grey(image, i - 1, j - 1) +
            (b2)*pixel_grey(image, i, j - 1) + (b3)*pixel_grey(image, i + 1, j - 1) + (b4)*pixel_grey(image, i + 2, j - 1) + (c0)*pixel_grey(image, i - 2, j) + (c1)*pixel_grey(image, i - 1, j) + (c2)*pixel_grey(image, i, j) +
            (c3)*pixel_grey(image, i + 1, j) + (c4)*pixel_grey(image, i + 2, j) + (d0)*pixel_grey(image, i - 2, j + 1) +
            (d1)*pixel_grey(image, i - 1, j + 1) + (d2)*pixel_grey(image, i, j + 1) + (d3)*pixel_grey(image, i + 1, j + 1) +
            (d4)*pixel_grey(image, i + 2, j + 1) + (e0)*pixel_grey(image, i - 2, j + 2) + (e1)*pixel_grey(image, i - 1, j + 2) +
            (e2)*pixel_grey(image, i, j + 2) + (e3)*pixel_grey(image, i + 1, j + 2) + (e4)*pixel_grey(image, i + 2, j + 2)) /
           25;
}

Uint8 kernel3x3(float a0,
                float a1,
                float a2,
                float b0,
                float b1,
                float b2,
                float c0,
                float c1,
                float c2,
                SDL_Surface *image, unsigned int i, unsigned int j)
{
    return ((a0)*pixel_grey(image, i - 1, j - 1) + (a1)*pixel_grey(image, i, j - 1) + (a2)*pixel_grey(image, i + 1, j - 1) +
            (b0)*pixel_grey(image, i - 1, j) + (b1)*pixel_grey(image, i, j) + (b2)*pixel_grey(image, i + 1, j) +
            (c0)*pixel_grey(image, i - 1, j + 1) + (c1)*pixel_grey(image, i, j + 1) + (c2)*pixel_grey(image, i + 1, j + 1)) /
           9;
}

/*
 * array_is_sorted(array, len): returns true if array is sorted in increasing
 * order
 */
int array_is_sorted(int array[], size_t len)
{
    for (size_t i = 0; i < len - 1; i++)
    {
        if (array[i] > array[i + 1])
        {
            return 1;
        }
    }
    return 0;
}

/* Min_array */

/*
 * array_min_index(array, len): returns the index of the min value of array
 */
size_t array_min_index(int array[], size_t t, size_t j)
{
    size_t sm_index = t;
    int smallest = array[t];

    for (size_t i = t; i < j; i++)
    {
        if (smallest > array[i])
        {
            smallest = array[i];
            sm_index = i;
        }
    }
    return sm_index;
}

size_t array_min_index_uint(size_t *array, size_t t, size_t j)
{
    size_t sm_index = t;
    Uint8 smallest = array[t];

    for (size_t i = t; i < j; i++)
    {
        if (smallest > array[i])
        {
            smallest = array[i];
            sm_index = i;
        }
    }
    return sm_index;
}
/* Selection sort */

/*
 * array_select_sort(array, len): sort array using select sort
 */
void array_select_sort(int array[], size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        int min = array_min_index(array, i, len);
        int i_val = array[i];
        int j_val = array[min];
        array[min] = i_val;
        array[i] = j_val;
    }
}

void array_select_sort_uint8(size_t *array, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        size_t min = array_min_index_uint(array, i, len);
        Uint8 i_val = array[i];
        Uint8 j_val = array[min];
        array[min] = i_val;
        array[i] = j_val;
    }
}

//Application of median filter 3X3 pixels
//The median value is the fifth in a sorted list of 9 values
void median(int w, int h, SDL_Surface *image)
{
    SDL_Surface *image_coppy = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    for (int i = 1; i < image->w - 1; i++)
    {
        for (int j = 1; j < image->h - 1; j++)
        {
            int table[9] = {pixel_grey(image, i - 1, j - 1), pixel_grey(image, i, j - 1), pixel_grey(image, i + 1, j - 1),
                            pixel_grey(image, i - 1, j), pixel_grey(image, i, j), pixel_grey(image, i + 1, j),
                            pixel_grey(image, i - 1, j + 1), pixel_grey(image, i, j + 1), pixel_grey(image, i + 1, j + 1)};
            array_select_sort(table, 9);
            int median_value = table[4];
            Uint32 pixel_new = SDL_MapRGB(image->format, median_value, median_value, median_value);
            put_pixel(image_coppy, i, j, pixel_new);
        }
    }
    SDL_SaveBMP(image_coppy, "C:/Users/vladi/SDL_test/src/median.bmp");
}
//Application of median filter 5X5 pixels
//The median value is the twelvth in a sorted list of 25 values
SDL_Surface *median5(SDL_Surface *image)
{
    int w = image->w;
    int h = image->h;
    SDL_Surface *image_coppy = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    for (int i = 1; i < w - 1; i++)
    {
        for (int j = 1; j < h - 1; j++)
        {
            int table[25] = {pixel_grey(image, i - 1, j - 1), pixel_grey(image, i, j - 1), pixel_grey(image, i + 1, j - 1),
                             pixel_grey(image, i - 1, j), pixel_grey(image, i, j), pixel_grey(image, i + 1, j),
                             pixel_grey(image, i - 1, j + 1), pixel_grey(image, i, j + 1), pixel_grey(image, i + 1, j + 1),
                             pixel_grey(image, i - 2, j - 2), pixel_grey(image, i - 1, j - 2), pixel_grey(image, i, j - 2),
                             pixel_grey(image, i + 1, j - 2), pixel_grey(image, i + 2, j - 2), pixel_grey(image, i - 2, j - 1),
                             pixel_grey(image, i + 2, j - 1), pixel_grey(image, i - 2, j), pixel_grey(image, i + 2, j),
                             pixel_grey(image, i - 2, j + 1), pixel_grey(image, i + 2, j + 1),
                             pixel_grey(image, i - 2, j + 2), pixel_grey(image, i - 1, j + 2),
                             pixel_grey(image, i, j + 2), pixel_grey(image, i + 1, j + 2), pixel_grey(image, i + 2, j + 2)};
            array_select_sort(table, 25);
            int median_value = table[12];
            Uint32 pixel_new = SDL_MapRGB(image->format, median_value, median_value, median_value);
            put_pixel(image_coppy, i, j, pixel_new);
        }
    }
    return image_coppy;
}
//Combines two images pixel by pixel , with corresponding pixel intensity;
//For exemple: You have a pixel on image one at (i,j) with values (10,10,10) (Of course we are in grey scale so: r = g =b)
// So we take 10 and we devide it by image intensity for exemple 2
//We get 10/2 = 5. And lets say we have a pixel on the second image at the same coordinates (i,j)
//It's value is 7 and we divide it by the intensity 1. 7/1 = 7 ;
// And so we finally add the two pixels and save them into a new image
// 5+7 = 12 and ve divide this by 2 ==> so 6;
//And we do this for each pixel
SDL_Surface *combine_images(SDL_Surface *image1, SDL_Surface *image2, int image1_intensity, int image2_intensity)
{
    int image_h = image1->h;
    int image_w = image1->w;

    for (int i = 1; i < image_w - 1; i++)
    {
        for (int j = 1; j < image_h - 1; j++)
        {
            Uint32 pixel_new = get_pixel(image1, i, j);
            Uint32 pixel_red = get_pixel(image2, i, j);
            if (pixel_red == SDL_MapRGB(image1->format, 255, 2, 2))
            {
                continue;
            }
            if (pixel_new == SDL_MapRGB(image1->format, 255, 2, 2))
            {
                put_pixel(image2, i, j, pixel_new);
            }
            else
            {
                Uint8 new_value = (pixel_grey(image1, i, j) / image1_intensity + pixel_grey(image2, i, j) / image2_intensity);
                Uint32 pixel_new = SDL_MapRGB(image1->format, new_value, new_value, new_value);
                put_pixel(image2, i, j, pixel_new);
            }
        }
    }
    return image2;
}
SDL_Surface *horizontal_edge_detection_sobel(SDL_Surface *image)
{
    int w = image->w;
    int h = image->h;
    SDL_Surface *image_coppy = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    for (int i = 1; i < w - 1; i++)
    {
        for (int j = 1; j < h - 1; j++)
        {
            Uint8 new_value = kernel3x3(
                1, 2, 1,
                0, 0, 0,
                -1, -2, -1,
                image, i, j);
            Uint32 pixel_new = SDL_MapRGB(image->format, new_value, new_value, new_value);
            put_pixel(image_coppy, i, j, pixel_new);
        }
    }
    SDL_SaveBMP(image_coppy, "C:/Users/vladi/SDL_test/src/hor.bmp");
    return image_coppy;
}
SDL_Surface *horizontal_edge_detection_sobel1(SDL_Surface *image)
{
    int w = image->w;
    int h = image->h;
    SDL_Surface *image_coppy = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    for (int i = 1; i < w - 1; i++)
    {
        for (int j = 1; j < h - 1; j++)
        {
            Uint8 new_value = kernel3x3(
                -1, -2, -1,
                0, 0, 0,
                1, 2, 1,
                image, i, j);
            Uint32 pixel_new = SDL_MapRGB(image->format, new_value, new_value, new_value);
            put_pixel(image_coppy, i, j, pixel_new);
        }
    }
    SDL_SaveBMP(image_coppy, "C:/Users/vladi/SDL_test/src/hor1.bmp");
    return image_coppy;
}
SDL_Surface *vertical_edge_detection_sobel(SDL_Surface *image)
{
    int w = image->w;
    int h = image->h;
    SDL_Surface *image_coppy = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    for (int i = 1; i < w - 1; i++)
    {
        for (int j = 1; j < h - 1; j++)
        {
            Uint8 new_value = kernel3x3(
                -1, 0, 1,
                -2, 0, 2,
                -1, 0, 1,
                image, i, j);
            Uint32 pixel_new = SDL_MapRGB(image->format, new_value, new_value, new_value);
            put_pixel(image_coppy, i, j, pixel_new);
        }
    }
    SDL_SaveBMP(image_coppy, "C:/Users/vladi/SDL_test/src/vert.bmp");
    return image_coppy;
}
SDL_Surface *vertical_edge_detection_sobel1(SDL_Surface *image)
{
    int w = image->w;
    int h = image->h;
    SDL_Surface *image_coppy = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    for (int i = 1; i < w - 1; i++)
    {
        for (int j = 1; j < h - 1; j++)
        {
            Uint8 new_value = kernel3x3(
                1, 0, -1,
                2, 0, -2,
                1, 0, -1,
                image, i, j);
            Uint32 pixel_new = SDL_MapRGB(image->format, new_value, new_value, new_value);
            put_pixel(image_coppy, i, j, pixel_new);
        }
    }
    SDL_SaveBMP(image_coppy, "C:/Users/vladi/SDL_test/src/vert1.bmp");
    return image_coppy;
}
SDL_Surface *combine_sobel(SDL_Surface *image_hor, SDL_Surface *image_vert)
{
    int h = image_hor->h;
    int w = image_hor->w;
    SDL_Surface *image_coppy = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    for (size_t i = 0; i < w; i++)
    {
        for (size_t j = 0; j < h; j++)
        {
            Uint8 hor_pix = pixel_grey(image_hor, i, j);
            Uint8 vert_pix = pixel_grey(image_vert, i, j);
            int squared = (int)vert_pix * (int)vert_pix + (int)hor_pix * (int)hor_pix;
            // printf("squared -> %d ",squared);
            int new_pix_val = (int)sqrt((double)squared);
            // printf("new_pix_val -> %d\n",new_pix_val);
            Uint32 new_pix = SDL_MapRGB(image_hor->format, new_pix_val, new_pix_val, new_pix_val);
            put_pixel(image_coppy, i, j, new_pix);
        }
    }
    return image_coppy;
}
SDL_Surface *sobel(SDL_Surface *image)
{
    int h = image->h;
    int w = image->w;
    SDL_Surface *image_hor = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    image_hor = horizontal_edge_detection_sobel(image);

    SDL_Surface *image_vert = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    image_vert = vertical_edge_detection_sobel(image);

    SDL_Surface *image_hor1 = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    image_hor1 = horizontal_edge_detection_sobel(image);

    SDL_Surface *image_vert1 = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    image_vert1 = vertical_edge_detection_sobel(image);

    SDL_Surface *sobel = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    sobel = combine_sobel(image_hor, image_vert);

    SDL_Surface *sobel1 = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    sobel1 = combine_sobel(image_hor1, image_vert1);

    SDL_Surface *sobel2 = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    sobel2 = combine_sobel(sobel1, sobel);

    SDL_FreeSurface(image_hor);
    SDL_FreeSurface(image_vert);
    SDL_FreeSurface(image_hor1);
    SDL_FreeSurface(image_vert1);
    SDL_FreeSurface(sobel);
    SDL_FreeSurface(sobel1);

    return sobel2;
}
//Function that founds borders bu which we will seacrh percentege
void detect_beg_end_incolumn(SDL_Surface *image, int percentage, int division, int borders[2], int column, int significant)
{
    int global = 1;
    int h = image->h;
    int black = 1; //bool
    int white = 1; //bool
    int first_cordinate = 0;
    int last_cordinate = 0;
    while (global < h)
    {
        if (black)
        {
            for (int i = 0; global < h && (pixel_grey(image, column, global) < division
                                           // ||
                                           //                                pixel_grey(image, column - 1, global) < division ||
                                           //                                pixel_grey(image, column + 1, global) < division
                                          );
                 i++)
            {
                global += 1;
            }
            white = 1;
            black = 0;
        }
        if (white)
        {
            int white_occurence = 0;
            for (int i = 0; global < h && (pixel_grey(image, column, global) >= division
                                           //  ||
                                           //                                pixel_grey(image, column - 1, global) >= division ||
                                           //                                pixel_grey(image, column + 1, global) >= division
                                          );
                 i++)
            {
                global += 1;
                white_occurence += 1;
            }

            if (white_occurence > significant)
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
    borders[0] = first_cordinate;
    borders[1] = last_cordinate;
}
void detect_beg_end_inline(SDL_Surface *image, int percentage, int division, int borders[2], int line, int significant)
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
            for (int i = 0; global < w && (pixel_grey(image, global, line) < division
                                           // ||
                                           //                                pixel_grey(image, global - 1, line) < division ||
                                           //                                pixel_grey(image, global + 1, line) < division
                                          );
                 i++)
            {
                global += 1;
            }
            white = 1;
            black = 0;
        }
        if (white)
        {
            int white_occurence = 0;
            for (int i = 0; global < w && (pixel_grey(image, global, line) >= division
                                           // ||
                                           //                                pixel_grey(image, global - 1, line) >= division ||
                                           //                                pixel_grey(image, global + 1, line) >= division
                                          );
                 i++)
            {
                global += 1;
                white_occurence += 1;
            }

            if (white_occurence > significant)
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
    borders[0] = first_cordinate;
    borders[1] = last_cordinate;
}
void trace_beg_end_line(SDL_Surface *image, int percentage, int division, int significant)
{
    int borders[2] = {0, 0};
    for (int i = 0; i < image->h; i++)
    {
        detect_beg_end_inline(image, percentage, division, borders, i, significant);
        for (int j = borders[0] + 1; j < borders[1]; j++)
        {
            Uint32 pixel1_new = SDL_MapRGB(image->format, 255, 2, 2);
            put_pixel(image, j, i, pixel1_new);
        }
    }
}

void trace_beg_end_column(SDL_Surface *image, int percentage, int division, int significant)
{
    int borders[2] = {0, 0};
    for (int i = 0; i < image->w; i++)
    {
        detect_beg_end_incolumn(image, percentage, division, borders, i, significant);
        for (int j = borders[0] + 1; j < borders[1]; j++)
        {
            Uint32 pixel1_new = SDL_MapRGB(image->format, 255, 2, 2);
            put_pixel(image, i, j, pixel1_new);
        }
    }
}

int median_image(SDL_Surface *image)
{
    int h = image->h;
    int w = image->w;
    int total = h * w;
    int *array = malloc(sizeof(int) * 256);
    for (size_t i = 0; i < 256; i++)
    {
        array[i] = 0;
    }
    int ptr = 0;
    int maxLevelValue = 0;

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            int h = 0xFF & pixel_grey(image, y, x);
            array[h]++;
            if (array[h] > maxLevelValue)
                maxLevelValue = array[h];
        }
    }

    float sum = 0;
    for (int t = 0; t < 256; t++)
        sum += t * array[t];

    float sumB = 0;
    int wB = 0;
    int wF = 0;

    float varMax = 0;
    int threshold = 0;

    for (int t = 0; t < 256; t++)
    {
        wB += array[t]; // Weight Background
        if (wB == 0)
            continue;

        wF = total - wB; // Weight Foreground
        if (wF == 0)
            break;

        sumB += (float)(t * array[t]);

        float mB = sumB / wB;         // Mean Background
        float mF = (sum - sumB) / wF; // Mean Foreground

        // Calculate Between Class Variance
        float varBetween = (float)wB * (float)wF * (mB - mF) * (mB - mF);

        // Check if new maximum found
        if (varBetween > varMax)
        {
            varMax = varBetween;
            threshold = t;
        }
    }

    // float *bin_edges = malloc(sizeof(float) * 256);
    // bin_edges[0] = 0.0;
    // float increment = 0.99609375;
    // printf("bin_edges values:\n");
    // for (size_t i = 1; i < 256; i++)
    // {
    //     bin_edges[i] = bin_edges[i - 1] + increment;
    //     // printf("bin_edges[%zu] -> %.2f :", i, bin_edges[i]);
    // }
    // printf("\n");
    // float *bin_mids = malloc(sizeof(float) * 256);
    // printf("bin_mids values:\n");
    // for (size_t i = 0; i < 256; i++)
    // {

    //     bin_mids[i] = (bin_edges[i] + bin_edges[i + 1]) / 2;
    //     // printf("bin_mids[%zu] -> %.2f :", i, bin_mids[i]);
    // }
    // printf("\n");
    // float *weight1 = malloc(sizeof(float) * 256);
    // printf("weight1 values:\n");
    // weight1[0] = array[0];
    // for (size_t i = 1; i < 256; i++)
    // {
    //     weight1[i] = array[i] + weight1[i - 1];
    //     // printf("weight1[%zu] -> %.2f :", i, weight1[i]);
    // }
    // printf("\n");
    // int total_sum = w*h;
    // float *weight2 = malloc(sizeof(float) * 256);
    // printf("weight2 values:\n");
    // weight2[0] = total_sum;
    // for (size_t i = 1; i < 256; i++)
    // {
    //     weight2[i] = weight2[i - 1] - array[i - 1];
    //     // printf("weight2[%zu] -> %.2f :", i, weight2[i]);
    // }
    // printf("\n");
    // float *histogram_bin_mids = malloc(sizeof(float) * 256);
    // printf("histogram_bin_mids values:\n");
    // for (size_t i = 0; i < 256; i++)
    // {
    //     histogram_bin_mids[i] = array[i] * bin_mids[i];
    //     // printf("histogram_bin_mids[%zu] -> %.2f :", i, histogram_bin_mids[i]);
    // }
    // printf("\n");
    // float *cumsum_mean1 = malloc(sizeof(float) * 256);
    // cumsum_mean1[0] = histogram_bin_mids[0];
    // for (size_t i = 1; i < 256; i++)
    // {
    //     cumsum_mean1[i] = cumsum_mean1[i - 1] + histogram_bin_mids[i];
    // }
    // float *cumsum_mean2 = malloc(sizeof(float) * 256);
    // cumsum_mean2[0] = histogram_bin_mids[255];
    // for (size_t i = 1, j = 254; i < 256 && j >= 0; i++, j--)
    // {
    //     cumsum_mean2[i] = cumsum_mean2[i - 1] + histogram_bin_mids[j];
    // }
    // float *mean1 = malloc(sizeof(float) * 256);
    // for (size_t i = 0; i < 256; i++)
    // {
    //     mean1[i] = cumsum_mean1[i] / weight1[i];
    // }
    // float *mean2 = malloc(sizeof(float) * 256);
    // for (size_t i = 0, j = 255; i < 256 && j >= 0; i++, j--)
    // {
    //     mean2[j] = cumsum_mean2[i] / weight2[j];
    // }
    // float *inter_class_variance = malloc(sizeof(float) * 255);
    // float dnum = 10000000000;
    // for (size_t i = 0; i < 255; i++)
    // {
    //     inter_class_variance[i] = ((weight1[i] * weight2[i] * (mean1[i] - mean2[i + 1])) / dnum) * (mean1[i] - mean2[i + 1]);
    // }
    // long double maxi = 0;

    // int getmax = 0;
    // for (int i = 0; i < 255; i++)
    // {
    //     if (maxi < inter_class_variance[i])
    //     {
    //         maxi = inter_class_variance[i];
    //         getmax = i;
    //     }
    // }
    // printf("%.2f", bin_mids[getmax]);
    // free(array);
    // free(bin_edges);
    // free(bin_mids);
    // free(weight1);
    // free(weight2);
    // free(histogram_bin_mids);
    // free(cumsum_mean1);
    // free(cumsum_mean2);
    // free(mean1);
    // free(mean2);
    // free(inter_class_variance);
    // return bin_mids[getmax];
    printf("%d", threshold);
    return threshold;
}

SDL_Surface *binarisation(SDL_Surface *image)
{
    int w = image->w;
    int h = image->h;
    int div = median_image(image);
    SDL_Surface *image_coppy = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    for (int i = 1; i < w - 1; i++)
    {
        for (int j = 1; j < h - 1; j++)
        {

            Uint8 new_value;
            if (pixel_grey(image, i, j) <= div)
            {
                new_value = 255;
            }
            else
            {
                new_value = 0;
            }
            Uint32 pixel_new = SDL_MapRGB(image->format, new_value, new_value, new_value);
            put_pixel(image_coppy, i, j, pixel_new);
        }
    }
    return image_coppy;
}

SDL_Surface *gaussian_blur(SDL_Surface *image)
{
    int h = image->h;
    int w = image->w;
    SDL_Surface *image_coppy = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    for (int i = 1; i < w - 1; i++)
    {
        for (int j = 1; j < h - 1; j++)
        {
            Uint8 new_value = kernel3x3(
                0.8, 1, 0.8,
                1, 1.2, 1,
                0.8, 1, 0.8,
                image, i, j);
            Uint32 pixel_new = SDL_MapRGB(image->format, new_value, new_value, new_value);
            put_pixel(image_coppy, i, j, pixel_new);
        }
    }
    return image_coppy;
}

void grey_scale(SDL_Surface *image)
{
    int w = image->w;
    int h = image->h;
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            Uint32 pixel = get_pixel(image, i, j);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            Uint8 average = 0.3 * r + 0.59 * g + 0.11 * b;
            Uint32 pixel_new = SDL_MapRGB(image->format, average, average, average);
            put_pixel(image, i, j, pixel_new);
        }
    }
}

SDL_Surface *horizontal_edge_detection1(SDL_Surface *image)
{
    int w = image->w;
    int h = image->h;
    SDL_Surface *image_coppy = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    for (int i = 1; i < w - 1; i++)
    {
        for (int j = 1; j < h - 1; j++)
        {
            //We know hat r = g = b HAVE TO OPTIMISE THAT

            Uint8 new_value = kernel3x3(
                1, 2, 1,
                0, 0, 0,
                -1, -2, -1,
                image, i, j);
            Uint32 pixel_new = SDL_MapRGB(image->format, new_value, new_value, new_value);
            put_pixel(image_coppy, i, j, pixel_new);
        }
    }
    trace_beg_end_line(image_coppy, 90, 50, h / 15);
    return image_coppy;
}
SDL_Surface *horizontal_edge_detection(SDL_Surface *image)
{
    int w = image->w;
    int h = image->h;
    SDL_Surface *image_coppy = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    for (int i = 1; i < w - 1; i++)
    {
        for (int j = 1; j < h - 1; j++)
        {
            //We know hat r = g = b HAVE TO OPTIMISE THAT
            Uint8 new_value = kernel3x3(
                -1, -2, -1,
                0, 0, 0,
                1, 2, 1,
                image, i, j);
            Uint32 pixel_new = SDL_MapRGB(image->format, new_value, new_value, new_value);
            put_pixel(image_coppy, i, j, pixel_new);
        }
    }
    trace_beg_end_line(image_coppy, 90, 50, h / 15);
    return image_coppy;
}
SDL_Surface *vertical_edge_detection(SDL_Surface *image)
{
    int w = image->w;
    int h = image->h;
    SDL_Surface *image_coppy = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    for (int i = 1; i < w - 1; i++)
    {
        for (int j = 1; j < h - 1; j++)
        {
            Uint8 new_value = kernel3x3(
                -1, 0, 1,
                -2, 0, 2,
                -1, 0, 1,
                image, i, j);
            Uint32 pixel_new = SDL_MapRGB(image->format, new_value, new_value, new_value);
            put_pixel(image_coppy, i, j, pixel_new);
        }
    }
    trace_beg_end_column(image_coppy, 90, 50, w / 15);
    return image_coppy;
}

SDL_Surface *vertical_edge_detection1(SDL_Surface *image)
{
    int w = image->w;
    int h = image->h;
    SDL_Surface *image_coppy = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    for (int i = 1; i < w - 1; i++)
    {
        for (int j = 1; j < h - 1; j++)
        {
            Uint8 new_value = kernel3x3(
                1, 0, -1,
                2, 0, -2,
                1, 0, -1,
                image, i, j);
            Uint32 pixel_new = SDL_MapRGB(image->format, new_value, new_value, new_value);
            put_pixel(image_coppy, i, j, pixel_new);
        }
    }
    trace_beg_end_column(image_coppy, 90, 50, w / 15);
    return image_coppy;
}

void run_through_vert(SDL_Surface *image, int h, int w, int tracing_size)
{
    int deplacement = 0;
    int occurances = 0;
    int current_occurances = 0;
    int most_green_i = 0;
    int first_green = 0;
    int last_green = 0;
    int save_green_last = 0;
    int save_green_first = 0;
    while (tracing_size + deplacement < h)
    {
        for (int i = deplacement; i < tracing_size + deplacement; i++)
        {
            for (int j = 0; j < w; j++)
            {
                Uint32 pixel = get_pixel(image, j, i);
                if (pixel == SDL_MapRGB(image->format, 2, 255, 25))
                {
                    current_occurances += 1;
                    last_green = j;
                    if (first_green == 0)
                    {
                        first_green = j;
                    }
                }
            }
        }
        if (occurances < current_occurances)
        {
            occurances = current_occurances;
            most_green_i = deplacement;
            save_green_last = last_green;
            save_green_first = first_green;
        }
        current_occurances = 0;
        first_green = 0;
        last_green = 0;
        deplacement += 1;
    }
    // printf("most_green_i: %d first: %d last: %d\n", most_green_i, save_green_first, save_green_last);
    for (int i = most_green_i; i < tracing_size + most_green_i; i++)
    {
        for (int j = save_green_first; j < save_green_last; j++)
        {
            Uint32 pixel_new = SDL_MapRGB(image->format, 100, 100, 20);
            put_pixel(image, j, i, pixel_new);
        }
    }
    SDL_SaveBMP(image, "C:/Users/vladi/SDL_test/src/vert_startline.bmp");
}

void run_through_vert1(SDL_Surface *image, int h, int w, int tracing_size)
{
    int deplacement = 1;
    int occurances = 0;
    int current_occurances = 0;
    int most_blue_i = 0;
    int first_blue = 0;
    int last_blue = 0;
    int save_blue_last = 0;
    int save_blue_first = 0;
    while (tracing_size + deplacement < h)
    {
        for (int i = deplacement; i < tracing_size + deplacement; i++)
        {
            for (int j = 0; j < w; j++)
            {
                Uint32 pixel = get_pixel(image, j, i);
                if (pixel == SDL_MapRGB(image->format, 2, 10, 255))
                {
                    current_occurances += 1;
                    last_blue = j;
                    if (first_blue == 0)
                    {
                        first_blue = j;
                    }
                }
            }
        }
        if (occurances < current_occurances)
        {
            occurances = current_occurances;
            most_blue_i = deplacement;
            save_blue_last = last_blue;
            save_blue_first = first_blue;
        }
        current_occurances = 0;
        first_blue = 0;
        last_blue = 0;
        deplacement += 1;
    }
    // printf("most_blue_i: %d first: %d last: %d\n", most_blue_i, save_blue_first, save_blue_last);
    for (int i = most_blue_i; i < tracing_size + most_blue_i; i++)
    {
        for (int j = save_blue_first; j < save_blue_last; j++)
        {
            Uint32 pixel_new = SDL_MapRGB(image->format, 100, 100, 20);
            put_pixel(image, j, i, pixel_new);
        }
    }
    SDL_SaveBMP(image, "C:/Users/vladi/SDL_test/src/vert_startline.bmp");
}
//A motive is something starting with a red pixel followed by n black pixels and ended by a red pixel
//The motive that will be underlined will be the most recurent one

SDL_Surface *detect_motive_hor(SDL_Surface *image)
{
    int h = image->h;
    int w = image->w;
    size_t array_size = h / 5;
    array_type *ar = malloc(sizeof(array_type));
    ar->len = array_size;
    ar->array_n = malloc(sizeof(int) * array_size);
    for (size_t i = 0; i < array_size; i++)
    {
        ar->array_n[i] = 0;
    }
    for (size_t i = 0; i < image->w; i++)
    {
        int j = 0;
        int serie = 0;
        Uint32 pixel = get_pixel(image, i, j);

        //If the pixel seen  not is red
        while (pixel != SDL_MapRGB(image->format, 255, 2, 2) && j < image->h)
        {
            j += 1;
            pixel = get_pixel(image, i, j);
        }
        while (j < image->h)
        {
            pixel = get_pixel(image, i, j);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            //scan untill the red ends
            while (pixel == SDL_MapRGB(image->format, 255, 2, 2) && j < image->h) //while the pixels are red in other words not black
            {
                j += 1;
                pixel = get_pixel(image, i, j);
            }
            while (pixel != SDL_MapRGB(image->format, 255, 2, 2) && j < image->h) //while the pixels are black in other words not red
            {
                serie += 1;
                j += 1;
                pixel = get_pixel(image, i, j);
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
    for (size_t i = 0; i < image->w; i++)
    {
        int j = 0;
        int serie = 0;
        Uint32 pixel = get_pixel(image, i, j);
        //If the pixel seen  not is red
        while (pixel != SDL_MapRGB(image->format, 255, 2, 2) && j < image->h)
        {
            j += 1;
            pixel = get_pixel(image, i, j);
        }
        while (j < image->h)
        {
            pixel = get_pixel(image, i, j);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            //scan untill the red ends
            while (pixel == SDL_MapRGB(image->format, 255, 2, 2) && j < image->h) //while the pixels are red in other words not black
            {
                j += 1;
                pixel = get_pixel(image, i, j);
            }
            while (pixel != SDL_MapRGB(image->format, 255, 2, 2) && j < image->h) //while the pixels are black in other words not red
            {
                serie += 1;
                j += 1;
                pixel = get_pixel(image, i, j);
                if (serie >= h / 5)
                {
                    serie = 0;
                    break;
                }
            }
            if (serie >= max_index - (h / 95) && serie <= max_index + (h / 95))
            {
                int actual_j = j - serie;
                while (j != actual_j)
                {

                    Uint32 pixel = SDL_MapRGB(image->format, 100, 0, 100);
                    put_pixel(image, i, actual_j, pixel);
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
    free(ar->array_n);
    return image;
}

SDL_Surface *detect_motive_vert(SDL_Surface *image)
{
    int h = image->h;
    int w = image->w;
    size_t array_size = (image->w) / 5;
    array_type *ar = malloc(sizeof(array_type));
    ar->len = array_size;
    ar->array_n = malloc(sizeof(int) * array_size);
    for (size_t i = 0; i < array_size; i++)
    {
        ar->array_n[i] = 0;
    }
    for (size_t i = 0; i < image->h; i++)
    {
        int j = 0;
        int serie = 0;
        Uint32 pixel = get_pixel(image, j, i);
        //If the pixel seen  not is red
        while (pixel != SDL_MapRGB(image->format, 255, 2, 2) && j < image->w)
        {
            j += 1;
            pixel = get_pixel(image, j, i);
        }
        while (j < image->w)
        {
            pixel = get_pixel(image, j, i);

            while (pixel == SDL_MapRGB(image->format, 255, 2, 2) && j < image->w) //while the pixels are red in other words not black
            {
                j += 1;
                pixel = get_pixel(image, j, i);
            }
            while (pixel != SDL_MapRGB(image->format, 255, 2, 2) && j < image->w) //while the pixels are black in other words not red
            {
                serie += 1;
                j += 1;
                pixel = get_pixel(image, j, i);
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
            if (j >= image->w)
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
    for (size_t i = 0; i < image->h; i++)
    {
        int j = 0;
        int serie = 0;
        Uint32 pixel = get_pixel(image, j, i);
        //If the pixel seen  not is red
        while (pixel != SDL_MapRGB(image->format, 255, 2, 2) && j < image->w)
        {
            j += 1;
            pixel = get_pixel(image, j, i);
        }
        while (j < image->w)
        {
            pixel = get_pixel(image, j, i);
            //scan untill the red ends
            while (pixel == SDL_MapRGB(image->format, 255, 2, 2) && j < image->w) //while the pixels are red in other words not black
                pixel = get_pixel(image, ++j, i);

            while (pixel != SDL_MapRGB(image->format, 255, 2, 2) && j < image->w) //while the pixels are black in other words not red
            {
                pixel = get_pixel(image, ++j, i);
                if (++serie >= w / 5)
                {
                    serie = 0;
                    break;
                }
            }

            if (serie >= max_index - (w / 95) && serie <= max_index + (w / 95))
            {
                int actual_j = j - serie - 1;
                while (j != ++actual_j)
                    if (get_pixel(image, actual_j, i) == SDL_MapRGB(image->format, 100, 0, 100))
                        put_pixel(image, actual_j, i, SDL_MapRGB(image->format, 100, 255, 100));
            }
            //We saw something else than black
            if (serie > 1)
            {
                ar->array_n[serie - 1]++;
                serie = 0;
            }
            if (j >= image->w)
                break;
        }
    }
    free(ar);
    free(ar->array_n);
    return image;
}

void combine_detections(SDL_Surface *image, SDL_Surface *image1)
{
    for (size_t i = 0; i < image->w; i++)
        for (size_t j = 0; j < image->h; j++)
            if (get_pixel(image, i, j) == SDL_MapRGB(image->format, 100, 0, 100))
                if (get_pixel(image1, i, j) == SDL_MapRGB(image1->format, 0, 255, 0))
                    put_pixel(image, i, j, SDL_MapRGB(image->format, 100, 255, 100));
    SDL_SaveBMP(image, "C:/Users/vladi/SDL_test/src/detect_all.bmp");
}

//Calling the function knowing we have just detected greeeeeeeeen
//It counts the number of greeeeeeeeen occurances in a hor line
size_t detect_green_hor(SDL_Surface *image, size_t x, size_t y)
{
    size_t count = 0;
    Uint32 pixel;
    while ((pixel = get_pixel(image, x++, y)) == SDL_MapRGB(image->format, 100, 255, 100))
        count++;
    return count;
}

//Calling the function knowing we have just detected greeeeeeeeen
//It counts the number of greeeeeeeeen occurances in a vert line
size_t detect_green_vert(SDL_Surface *image, size_t x, size_t y)
{
    size_t count = 0;
    Uint32 pixel;
    while ((pixel = get_pixel(image, x, y++)) == SDL_MapRGB(image->format, 100, 255, 100))
        count++;
    return count;
}

char *create_str(int number)
{
    char str_path[] = "C:/Users/vladi/SDL_test/src/num";
    size_t str_path_len = 0;
    while (str_path[str_path_len] != 0)
        str_path_len++;

    char str_bmp[] = ".bmp";
    size_t str_bmp_len = 0;

    while (str_bmp[str_bmp_len] != 0)
        str_bmp_len++;

    int rem_num = number;
    size_t count = 0;
    while (number != 0)
    {
        count++;
        number /= 10;
    }

    char *str = malloc((count + 1) * sizeof(char));
    str[count] = '\0';

    for (size_t i = count; i > 0; i--)
    {
        str[i - 1] = (rem_num - (rem_num / 10) * 10) + '0';
        rem_num /= 10;
    }

    size_t str_path_malloc_len = str_path_len + count + str_bmp_len;
    char *str_path_malloc = malloc((str_path_len + count + str_bmp_len) * sizeof(char));
    str_path_malloc[str_path_malloc_len] = '\0';

    for (size_t i = str_bmp_len; i > 0; i--)
        str_path_malloc[--str_path_malloc_len] = str_bmp[i - 1];

    for (size_t i = count; i > 0; i--)
        str_path_malloc[--str_path_malloc_len] = str[i - 1];

    free(str);

    for (size_t i = str_path_len; i > 0; i--)
        str_path_malloc[--str_path_malloc_len] = str_path[i - 1];

    return str_path_malloc;
}

void detect_green(SDL_Surface *image_green, SDL_Surface *original)
{
    int h = image_green->h;
    int w = image_green->w;
    int count_numbers = 0;
    size_t new_j = 0;
    size_t i = 0;
    while (i < h)
    {
        size_t j = new_j;
        while (j < w)
        {
            Uint32 pixel = get_pixel(image_green, i, j);
            if (pixel == SDL_MapRGB(image_green->format, 100, 255, 100))
            {
                printf("Corner i %zu, j %zu\n", i, j);
                size_t h = detect_green_vert(image_green, i, j);
                size_t v = detect_green_hor(image_green, i, j);

                SDL_Surface *number = SDL_CreateRGBSurface(0, v, h, 32, 0, 0, 0, 0);
                size_t index_j = h + j;
                size_t index_i = v + i;
                // printf("index_i %zu, index_j %zu\n", index_i, index_j);
                size_t remeber_i = i;
                size_t remebre_j = j;
                size_t num_i = 0;
                size_t num_j = 0;
                while (j < index_j)
                {
                    while (i < index_i)
                    {
                        Uint32 pixel1 = SDL_MapRGB(image_green->format, 0, 255, 0);
                        pixel = get_pixel(original, i, j);
                        put_pixel(number, num_i, num_j, pixel);
                        put_pixel(original, i, j, pixel1);
                        i += 1;
                        num_i += 1;
                    }
                    num_i = 0;
                    i = remeber_i;
                    j += 1;
                    num_j += 1;
                    char *str = create_str(count_numbers);
                    SDL_SaveBMP(number, str);
                    free(str);
                }
                SDL_FreeSurface(number);
                count_numbers += 1;

                if (count_numbers % 9 == 0)
                {
                    i = 0;
                    j = index_j;
                    new_j = index_j;
                }
                if (count_numbers % 9 != 0)
                {
                    i = index_i;
                    j = remebre_j;
                }
                pixel = get_pixel(image_green, i, j);
            }
            j += 1;
        }
        i += 1;
    }
}
int *new_sheer(double angle, int x, int y)
{
    int *x_and_y = malloc(sizeof(int) * 2);
    //sheer 1
    double intan = angle / 2.f;
    // printf("intan -> %.6f\n",intan);
    double tangent = tan(intan);
    // printf("tangent for angel -> %f: is -> %.6f\n",angle,tangent);
    int new_x = round((double)x - ((double)y * tangent));
    // printf("new_x -> %d \n",new_x);
    int new_y = y;
    // printf("new_y -> %d \n",new_y);
    //sheer 2
    new_y = round((double)new_x * sin(angle) + (double)new_y);
    // printf("new_y -> %d \n",new_y);
    //sheer 3
    new_x = round((double)new_x - ((double)new_y * tangent));
    // printf("new_x -> %d \n",new_x);
    x_and_y[0] = new_x;
    x_and_y[1] = new_y;
    return x_and_y;
}
SDL_Surface *rotate(SDL_Surface *image, double angle)
{
    int *x_and_y = malloc(sizeof(int) * 2);
    angle = angle / 57.29578;
    // printf("angle %f\n", angle);
    double cosine = cos(angle);
    // printf("cosine %f\n", cosine);
    double sinus = sin(angle);
    // printf("sinus %f\n", sinus);
    int height = image->h;
    int width = image->w;
    int new_height = round((abs(height * cosine) + abs(width * sinus))) +1;
    printf("%d new_height\n", new_height);
    int new_width = round((abs(width * cosine) + abs(height * sinus))) +1;
    printf("%d new_width\n", new_width);
    SDL_Surface *output_image = SDL_CreateRGBSurface(0, new_width+100 , new_height+100 , 32, 0, 0, 0, 0);

    int original_center_height = round(((height + 1) / 2) - 1);
    printf("original_centre_height ->  %zu :\n", original_center_height);
    int original_center_width = round(((width + 1) / 2) - 1);
    printf("original_centre_width ->  %zu :\n", original_center_width);
    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            int y = height - 1 - i - original_center_height;
            int x = width - 1 - j - original_center_width;
            // printf("y -> %d : x -> %d : \n",y,x);
            x_and_y = new_sheer(angle, x, y);
            int new_x = x_and_y[0];
            int new_y = x_and_y[1];
            // printf("new_y -> %d : new_x -> %d : \n",new_x,new_y);
            new_y = new_height - new_y;
            new_x = new_width - new_x;
            printf("new_y -> %d : new_x -> %d : after normalisation\n",new_y,new_x);
            Uint32 pixel = get_pixel(image, i, j);
            put_pixel(output_image, new_y, new_x, pixel);
        }
    }
    free(x_and_y);
    return output_image;
}


SDL_Surface *rotate_QUENTIN(SDL_Surface *image, double angle)
{
    angle = angle / 57.29578;
    double cosine = cos(angle);
    printf("cos -> %f",cosine);
    double sinus = sin(angle);
    printf("sin -> %f",sinus);
    int height = image->h;
    int width = image->w;
    int new_height = round((abs(height * cosine) + abs(width * sinus))) + 1;
    printf("%d new_height\n", new_height);
    int new_width = round((abs(width * cosine) + abs(height * sinus))) + 1;
    printf("%d new_width\n", new_width);
    SDL_Surface *output_image = SDL_CreateRGBSurface(0, new_width+width , new_height+height , 32, 0, 0, 0, 0);
    double y0 = round(((height + 1) / 2) - 1);
    printf("y0 ->  %f :\n", y0);
    double x0 = round(((width + 1) / 2) - 1);
    printf("x0 ->  %f :\n", x0);
    for (double i = 0; i < height; i++)
    {
        for (double j = 0; j < width; j++)
        {
            double x2 = cosine * (j - x0) + sinus * (i - y0) + new_height;
            double y2 = -sinus * (j - x0) + cosine * (i - y0) + new_width;
            printf("x2 -> %f, y2 -> %f\n",x2,y2);
            Uint32 pixel = get_pixel(image, i, j);
            put_pixel(output_image, (int)y2, (int)x2, pixel);
        }
    }
    return output_image;
}


int main(int argc, char *argv[])
{
    int quit = 0;
    SDL_Event event;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Sudoku",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 900, 900, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Surface *image = SDL_LoadBMP(argv[1]);

    //GreyScale for image original
        grey_scale(image);
        image = rotate(image, 120);
        // SDL_Surface *after_gaussian_blur = gaussian_blur(image);
        // after_gaussian_blur = gaussian_blur(after_gaussian_blur);
        // SDL_SaveBMP(after_gaussian_blur, "C:/Users/vladi/SDL_test/src/gauss.bmp");
        // SDL_Surface *after_median = median5(after_gaussian_blur);
        // SDL_SaveBMP(after_median, "C:/Users/vladi/SDL_test/src/median.bmp");
        // after_median = binarisation(after_median);
        // SDL_SaveBMP(after_median, "C:/Users/vladi/SDL_test/src/binary.bmp");
        // after_median = sobel(after_median);
        // SDL_SaveBMP(after_median, "C:/Users/vladi/SDL_test/src/sobel.bmp");

        // after_median = binarisation(after_median);
        // SDL_SaveBMP(after_median, "C:/Users/vladi/SDL_test/src/binary.bmp");

        // // SDL_SaveBMP(after_median,"C:/Users/vladi/SDL_test/src/binary.bmp");

        // //Detect horizontal edges and save in hor
        // SDL_Surface *horizontal_right = horizontal_edge_detection(after_median);
        // SDL_SaveBMP(horizontal_right, "C:/Users/vladi/SDL_test/src/hor_right.bmp");
        // SDL_Surface *horizontal_left = horizontal_edge_detection1(after_median);
        // SDL_SaveBMP(horizontal_left, "C:/Users/vladi/SDL_test/src/hor_left.bmp");
        // SDL_Surface *horizontal_full = combine_images(horizontal_left, horizontal_right, 1, 1);
        // SDL_SaveBMP(horizontal_full, "C:/Users/vladi/SDL_test/src/hor_full.bmp");

        // // Detect vertical edges and save in vert
        // SDL_Surface *vertical_right = vertical_edge_detection(after_median);
        // SDL_SaveBMP(vertical_right, "C:/Users/vladi/SDL_test/src/vert_right.bmp");
        // SDL_Surface *vertical_left = vertical_edge_detection1(after_median);
        // SDL_SaveBMP(vertical_left, "C:/Users/vladi/SDL_test/src/vert_left.bmp");
        // SDL_Surface *vertical_full = combine_images(vertical_right, vertical_left, 1, 1);
        // SDL_SaveBMP(vertical_full, "C:/Users/vladi/SDL_test/src/vert_full.bmp");

        // SDL_Surface *all = combine_images(vertical_full, horizontal_full, 1, 1);
        // SDL_SaveBMP(all, "C:/Users/vladi/SDL_test/src/all.bmp");
        // SDL_Surface *image_hor = detect_motive_hor(all);
        // SDL_SaveBMP(image_hor, "C:/Users/vladi/SDL_test/src/hor.bmp");
        // SDL_Surface *image_vert = detect_motive_vert(all);
        // SDL_SaveBMP(image_vert, "C:/Users/vladi/SDL_test/src/vert.bmp");
        // detect_green(image_vert, image);
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
        // SDL_FreeSurface(after_median);
        // SDL_FreeSurface(after_gaussian_blur);
        // SDL_FreeSurface(image_hor);
        // SDL_FreeSurface(image_vert);
        // SDL_FreeSurface(horizontal_right);
        // SDL_FreeSurface(horizontal_left);
        // SDL_FreeSurface(horizontal_full);
        // SDL_FreeSurface(vertical_right);
        // SDL_FreeSurface(vertical_left);
        // SDL_FreeSurface(vertical_full);
        // SDL_FreeSurface(all);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
}