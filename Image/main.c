
//gcc src/*.c -o bin/prog -I include -L lib -lmingw32 -lSDL2main -lSDL2

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <math.h>
#include "basic_pixel.h"
#include "kernel.h"
#include"primary_filters.h"
#include "median_filters.h"
#include "otsu_binary.h"
#include "detect_trace.h"
#include "detect_lines.h"
#include "rotation.h"
#include "sobel.h"
#include "final_detection.h"
#include "hough_functions.h"

typedef struct Sobel
{
    SDL_Surface *gradient_intensity;
    double *directions;
} Sobel;



typedef struct Couple
{
    double theta;
    double ro;
    int occurence;
} Couple;

//Combines two images pixel by pixel , with corresponding pixel intensity;
//For exemple: You have a pixel on image one at (i,j) with values (10,10,10) (Of course we are in grey scale so: r = g =b)
// So we take 10 and we devide it by image intensity for exemple 2
//We get 10/2 = 5. And lets say we have a pixel on the second image at the same coordinates (i,j)
//It's value is 7 and we divide it by the intensity 1. 7/1 = 7 ;
// And so we finally add the two pixels and save them into a new image
// 5+7 = 12 and ve divide this by 2 ==> so 6;
//And we do this for each pixel
// SDL_Surface *combine_images(SDL_Surface *image1, SDL_Surface *image2, int image1_intensity, int image2_intensity)
// {
//     int image_h = image1->h;
//     int image_w = image1->w;

//     for (int i = 1; i < image_w - 1; i++)
//     {
//         for (int j = 1; j < image_h - 1; j++)
//         {
//             Uint32 pixel_new = get_pixel(image1, i, j);
//             Uint32 pixel_red = get_pixel(image2, i, j);
//             if (pixel_red == SDL_MapRGB(image1->format, 255, 2, 2))
//             {
//                 continue;
//             }
//             if (pixel_new == SDL_MapRGB(image1->format, 255, 2, 2))
//             {
//                 put_pixel(image2, i, j, pixel_new);
//             }
//             else
//             {
//                 Uint8 new_value = (pixel_grey(image1, i, j) / image1_intensity + pixel_grey(image2, i, j) / image2_intensity);
//                 Uint32 pixel_new = SDL_MapRGB(image1->format, new_value, new_value, new_value);
//                 put_pixel(image2, i, j, pixel_new);
//             }
//         }
//     }
//     return image2;
// // }
// Sobel *combine_sobel_theta(SDL_Surface *image_hor, SDL_Surface *image_vert)
// {
//     int h = image_hor->h;
//     int w = image_hor->w;
//     double *directions = malloc(sizeof(double) * w * h);
//     Sobel *sobel_struct = malloc(sizeof(Sobel));

//     SDL_Surface *image_coppy = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
//     for (size_t i = 0; i < w; i++)
//     {
//         for (size_t j = 0; j < h; j++)
//         {
//             Uint8 hor_pix = pixel_grey(image_hor, i, j);
//             Uint8 vert_pix = pixel_grey(image_vert, i, j);
//             double theta = atan2(hor_pix, vert_pix);
//             double angle = theta * 180. / (double)M_PI;
//             if (angle == 0)
//             {
//                 angle += 180.;
//             }
//             directions[i * h + j] = angle;
//             int squared = (int)vert_pix * (int)vert_pix + (int)hor_pix * (int)hor_pix;
//             int new_pix_val = (int)sqrt((double)squared);
//             Uint32 new_pix = SDL_MapRGB(image_hor->format, new_pix_val, new_pix_val, new_pix_val);
//             put_pixel(image_coppy, i, j, new_pix);
//         }
//     }
//     sobel_struct->directions = directions;
//     sobel_struct->gradient_intensity = image_coppy;
//     return sobel_struct;
// }

// SDL_Surface *sobel_theta(SDL_Surface *image)
// {
//     int h = image->h;
//     int w = image->w;
//     SDL_Surface *non_maximum = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
//     SDL_Surface *image_hor = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
//     image_hor = horizontal_edge_detection_sobel(image);
//     SDL_Surface *image_vert = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
//     image_vert = vertical_edge_detection_sobel(image);

//     SDL_Surface *sobel_gradient = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
//     Sobel *sobel_struct = combine_sobel_theta(image_hor, image_vert);
//     sobel_gradient = sobel_struct->gradient_intensity;
//     double *directions = sobel_struct->directions;

//     SDL_FreeSurface(image_hor);
//     SDL_FreeSurface(image_vert);
//     for (size_t i = 1; i < w - 1; i++)
//     {
//         for (size_t j = 1; j < h - 1; j++)
//         {
//             double dir = directions[i * h + j];
//             Uint8 q = 255;
//             Uint8 r = 255;
//             if (((0 <= dir) && (dir < 22.5)) || ((157.5 <= dir) && (dir <= 180)))
//             {
//                 q = pixel_grey(sobel_gradient, i, j + 1);
//                 r = pixel_grey(sobel_gradient, i, j - 1);
//             }
//             if ((22.5 <= dir) && (dir < 67.5))
//             {
//                 q = pixel_grey(sobel_gradient, i + 1, j - 1);
//                 r = pixel_grey(sobel_gradient, i - 1, j + 1);
//             }
//             if ((67.5 <= dir) && (dir < 112.5))
//             {
//                 q = pixel_grey(sobel_gradient, i + 1, j);
//                 r = pixel_grey(sobel_gradient, i - 1, j);
//             }
//             if ((112.5 <= dir) && (dir < 157.5))
//             {
//                 q = pixel_grey(sobel_gradient, i - 1, j - 1);
//                 r = pixel_grey(sobel_gradient, i + 1, j + 1);
//             }
//             if ((pixel_grey(sobel_gradient, i, j) >= q) && (pixel_grey(sobel_gradient, i, j) >= r))
//             {
//                 Uint8 new_pix_val = pixel_grey(sobel_gradient, i, j);
//                 Uint32 new_pix = SDL_MapRGB(non_maximum->format, new_pix_val, new_pix_val, new_pix_val);
//                 put_pixel(non_maximum, i, j, new_pix);
//             }
//             else
//             {
//                 Uint8 new_pix_val = 0;
//                 Uint32 new_pix = SDL_MapRGB(non_maximum->format, new_pix_val, new_pix_val, new_pix_val);
//                 put_pixel(non_maximum, i, j, new_pix);
//             }
//         }
//     }
//     free(sobel_struct);
//     return non_maximum;
// }

// SDL_Surface *hysteris(SDL_Surface *image)
// {
//     int h = image->h;
//     int w = image->w;
//     SDL_Surface *image_coppy = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
//     for (size_t i = 1; i < w - 1; i++)
//     {
//         for (size_t j = 1; j < h - 1; j++)
//         {
//             Uint8 new_pix_val = pixel_grey(image, i, j);
//             if (new_pix_val < 200)
//             {
//                 if ((pixel_grey(image, i + 1, j - 1) >= 200) || (pixel_grey(image, i + 1, j) >= 200) ||
//                     (pixel_grey(image, i + 1, j + 1) >= 200) || (pixel_grey(image, i, j - 1) >= 200) ||
//                     (pixel_grey(image, i, j + 1) >= 200) || (pixel_grey(image, i - 1, j - 1) >= 200) ||
//                     (pixel_grey(image, i - 1, j) >= 200) || (pixel_grey(image, i - 1, j + 1) >= 200))
//                 {
//                     Uint32 new_pix = SDL_MapRGB(image->format, 0, 0, 0);
//                     put_pixel(image_coppy, i, j, new_pix);
//                 }
//                 else
//                 {
//                     Uint32 new_pix = SDL_MapRGB(image->format, 255, 255, 255);
//                     put_pixel(image_coppy, i, j, new_pix);
//                 }
//             }
//         }
//     }
//     return image_coppy;
// }

// SDL_Surface *combine_brightest(SDL_Surface *image_hor, SDL_Surface *image_vert)
// {
//     int h = image_hor->h;
//     int w = image_hor->w;
//     SDL_Surface *image_coppy = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
//     for (size_t i = 0; i < w; i++)
//     {
//         for (size_t j = 0; j < h; j++)
//         {
//             Uint8 hor_pix = pixel_grey(image_hor, i, j);
//             Uint8 vert_pix = pixel_grey(image_vert, i, j);
//             Uint8 new_pix_val = hor_pix < vert_pix ? vert_pix : hor_pix;
//             Uint32 new_pix = SDL_MapRGB(image_hor->format, new_pix_val, new_pix_val, new_pix_val);
//             put_pixel(image_coppy, i, j, new_pix);
//         }
//     }
//     return image_coppy;
// }

//Function that finds borders by which we will seacrh percentege
// It aplies a very simple principle, preparing the Meshcheriakov
// Detection algorithm.
/* It traces through a colum of pixels on an image.
    And detects significant portions of white,
    Here is an exemple to better understand how it works:

    We are going to represent our column by an array of gray_scale values 
    for an easier understanding we will consider this column containing only two colours,
    full black(1) and full white(0).

    [1,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,1,0,0,0,1]

    The algorithm will increment the pointer until we see the first white pixel,
    we will than see how many whites follow that black, 
    and if that number of whites is significant we place the first_cordinate
    pointer at the beggining of that pattern.
    We continue to scan through the image, until the last significant pattern.

    With our exemple if significant = 3;
    We will have first_cordinate = 3
    and last_coordinate = 20

    So the algorith concludes on the existence of a line.
    
// */
// void detect_beg_end_incolumn(SDL_Surface *image, int percentage, int division, int borders[2], int column, int significant)
// {
//     int global = 1;
//     int h = image->h;
//     int black = 1; //bool
//     int white = 1; //bool
//     //index of the beggining of a pattern
//     int first_cordinate = 0;
//     //index of the end of a pattern
//     int last_cordinate = 0;
//     // Run throught the whole line
//     while (global < h)
//     {
//         //Scan throught black
//         if (black)
//         {
//             for (int i = 0; global < h && (pixel_grey(image, column, global) < division); i++)
//             {
//                 global += 1;
//             }
//             // Until you find something white
//             white = 1;
//             black = 0;
//         }
//         //We know we found something white
//         if (white)
//         {
//             //We are going to count the number of occurencies of white
//             int white_occurence = 0;
//             for (int i = 0; global < h && (pixel_grey(image, column, global) >= division); i++)
//             {
//                 //INcrement the index to look at the next pixel
//                 global += 1;
//                 //increment the white occurence
//                 white_occurence += 1;
//             }
//             //If the occurence of white can be considered as significant for this image
//             if (white_occurence > significant)
//             {
//                 //If it is the first white patter we encounter
//                 if (first_cordinate == 0)
//                 {
//                     first_cordinate = global - white_occurence;
//                     last_cordinate = global;
//                 }
//                 //Otherwise just move the last-point pointer
//                 else
//                 {
//                     last_cordinate = global;
//                 }
//             }

//             white = 0;
//             black = 1;
//         }
//     }
//     borders[0] = first_cordinate;
//     borders[1] = last_cordinate;
// }
// //Exactly the same algorithm as the column detection but applied to a vertical line

// void detect_beg_end_inline(SDL_Surface *image, int percentage, int division, int borders[2], int line, int significant)
// {
//     int global = 1;
//     int w = image->w;
//     int black = 1; //bool
//     int white = 1; //bool
//     int first_cordinate = 0;
//     int last_cordinate = 0;
//     while (global < w)
//     {
//         if (black)
//         {
//             for (int i = 0; global < w && (pixel_grey(image, global, line) < division); i++)
//             {
//                 global += 1;
//             }
//             white = 1;
//             black = 0;
//         }
//         if (white)
//         {
//             int white_occurence = 0;
//             for (int i = 0; global < w && (pixel_grey(image, global, line) >= division); i++)
//             {
//                 global += 1;
//                 white_occurence += 1;
//             }

//             if (white_occurence > significant)
//             {
//                 if (first_cordinate == 0)
//                 {
//                     first_cordinate = global - white_occurence;
//                     last_cordinate = global;
//                 }
//                 else
//                 {
//                     last_cordinate = global;
//                 }

//                 white = 0;
//                 black = 1;
//             }

//             else
//             {
//                 white = 0;
//                 black = 1;
//             }
//         }
//     }
//     borders[0] = first_cordinate;
//     borders[1] = last_cordinate;
// }
// void trace_beg_end_line(SDL_Surface *image, int percentage, int division, int significant)
// {
//     int borders[2] = {0, 0};
//     for (int i = 0; i < image->h; i++)
//     {
//         detect_beg_end_inline(image, percentage, division, borders, i, significant);
//         for (int j = borders[0] + 1; j < borders[1]; j++)
//         {
//             Uint32 pixel1_new = SDL_MapRGB(image->format, 255, 2, 2);
//             put_pixel(image, j, i, pixel1_new);
//         }
//     }
// }

// void trace_beg_end_column(SDL_Surface *image, int percentage, int division, int significant)
// {
//     int borders[2] = {0, 0};
//     for (int i = 0; i < image->w; i++)
//     {
//         detect_beg_end_incolumn(image, percentage, division, borders, i, significant);
//         for (int j = borders[0] + 1; j < borders[1]; j++)
//         {
//             Uint32 pixel1_new = SDL_MapRGB(image->format, 255, 2, 2);
//             put_pixel(image, i, j, pixel1_new);
//         }
//     }
// }

// int otsu_threshold(SDL_Surface *image)
// {
//     int h = image->h;
//     int w = image->w;
//     int pixel_number = h * w;
//     double mean_weight = 1.0 / (double)pixel_number;
//     //You need to calculate histogramm here
//     int *histogram = malloc(sizeof(int) * 256);
//     for (size_t i = 0; i < 256; i++)
//     {
//         histogram[i] = 0;
//     }
//     int maxLevelValue = 0;

//     for (int y = 0; y < h; y++)
//     {
//         for (int x = 0; x < w; x++)
//         {
//             int h = 0xFF & pixel_grey(image, w, y);
//             histogram[h]++;
//             if (histogram[h] > maxLevelValue)
//                 maxLevelValue = histogram[h];
//         }
//     }
//     int final_thresh = -1;
//     int final_value = -1;
//     //The intensity array is initialised
//     int *intensity_arr = malloc(sizeof(int) * 256);
//     for (size_t i = 0; i < 256; i++)
//     {
//         intensity_arr[i] = i;
//     }
//     //We will now take each value of bins
//     for (size_t t = 0; t < 256; t++)
//     {
//         int pcb = 0;
//         int pcf = 0;
//         for (size_t i = 0; i < t; i++)
//         {
//             pcb += histogram[i];
//         }
//         printf("pcb sum for %zu is %d which makes %f in double\n", t, pcb, (double)pcb);
//         for (size_t i = t + 1; i < 256; i++)
//         {
//             pcf += histogram[i];
//         }
//         printf("pcf sum for %zu is %d which makes %f in double\n", t, pcf, (double)pcf);
//         double Wb = (double)pcb * mean_weight;
//         double Wf = (double)pcf * mean_weight;
//         printf("Wb -> %f , Wf -> %f\n", Wb, Wf);
//         double mub = (double)pcb / t;
//         double muf = (double)pcf / (255 - t);
//         printf("mub -> %f , muf -> %f\n", mub, muf);
//         double value = Wb * Wf * (mub - muf) * (mub - muf);
//         printf("value -> %f\n", value);
//         if (value > final_value)
//         {
//             final_thresh = t;
//             final_value = value;
//         }
//     }
//     free(histogram);
//     free(intensity_arr);
//     printf("%d\n", final_thresh);
// }

// int median_image(SDL_Surface *image)
// {
//     int h = image->h;
//     int w = image->w;
//     int tot = w * h;
//     int *array = malloc(sizeof(int) * 256);
//     for (size_t i = 0; i < 256; i++)
//     {
//         array[i] = 0;
//     }
//     int maxLevelValue = 0;

//     for (int y = 0; y < h; y++)
//     {
//         for (int x = 0; x < w; x++)
//         {
//             int h = 0xFF & pixel_grey(image, w, y);
//             array[h]++;
//             if (array[h] > maxLevelValue)
//                 maxLevelValue = array[h];
//         }
//     }

//     double sum = 0;
//     for (int t = 0; t < 256; t++)
//         sum += t * array[t];

//     double sumB = 0;
//     int wB = 0;
//     int wF = 0;

//     double varMax = 0;
//     int threshold = 0;

//     for (int t = 0; t < 256; t++)
//     {
//         wB += array[t]; // Weight Background
//         if (wB == 0)
//             continue;

//         wF = (tot)-wB; // Weight Foreground
//         if (wF == 0)
//             break;

//         sumB += (double)(t * array[t]);

//         double mB = sumB / wB;         // Mean Background
//         double mF = (sum - sumB) / wF; // Mean Foreground

//         // Calculate Between Class Variance
//         float varBetween = (double)wB * (double)wF * (mB - mF) * (mB - mF);

//         // Check if new maximum found
//         if (varBetween > varMax)
//         {
//             varMax = varBetween;
//             threshold = t;
//         }
//     }
//     printf("%d", threshold);
//     return threshold;

//     // float *bin_edges = malloc(sizeof(float) * 256);
//     // bin_edges[0] = 0.0;
//     // float increment = 0.99609375;
//     // printf("bin_edges values:\n");
//     // for (size_t i = 1; i < 256; i++)
//     // {
//     //     bin_edges[i] = bin_edges[i - 1] + increment;
//     //     printf("bin_edges[%zu] -> %.2f :", i, bin_edges[i]);
//     // }
//     // printf("\n");
//     // float *bin_mids = malloc(sizeof(float) * 256);
//     // printf("bin_mids values:\n");
//     // for (size_t i = 0; i < 256; i++)
//     // {

//     //     bin_mids[i] = (bin_edges[i] + bin_edges[i + 1]) / 2;
//     //     printf("bin_mids[%zu] -> %.2f :", i, bin_mids[i]);
//     // }
//     // printf("\n");
//     // float *weight1 = malloc(sizeof(float) * 256);
//     // printf("weight1 values:\n");
//     // weight1[0] = array[0];
//     // for (size_t i = 1; i < 256; i++)
//     // {
//     //     weight1[i] = array[i] + weight1[i - 1];
//     //     printf("weight1[%zu] -> %.2f :", i, weight1[i]);
//     // }
//     // printf("\n");
//     // int total_sum = w * h;
//     // float *weight2 = malloc(sizeof(float) * 256);
//     // printf("weight2 values:\n");
//     // weight2[0] = total_sum;
//     // for (size_t i = 1; i < 256; i++)
//     // {
//     //     weight2[i] = weight2[i - 1] - array[i - 1];
//     //     printf("weight2[%zu] -> %.2f :", i, weight2[i]);
//     // }
//     // printf("\n");
//     // float *histogram_bin_mids = malloc(sizeof(float) * 256);
//     // printf("histogram_bin_mids values:\n");
//     // for (size_t i = 0; i < 256; i++)
//     // {
//     //     histogram_bin_mids[i] = array[i] * bin_mids[i];
//     //     printf("histogram_bin_mids[%zu] -> %.2f :", i, histogram_bin_mids[i]);
//     // }
//     // printf("\n");
//     // float *cumsum_mean1 = malloc(sizeof(float) * 256);
//     // cumsum_mean1[0] = histogram_bin_mids[0];
//     // for (size_t i = 1; i < 256; i++)
//     // {
//     //     cumsum_mean1[i] = cumsum_mean1[i - 1] + histogram_bin_mids[i];
//     // }
//     // float *cumsum_mean2 = malloc(sizeof(float) * 256);
//     // cumsum_mean2[0] = histogram_bin_mids[255];
//     // for (size_t i = 1, j = 254; i < 256 && j >= 0; i++, j--)
//     // {
//     //     cumsum_mean2[i] = cumsum_mean2[i - 1] + histogram_bin_mids[j];
//     // }
//     // float *mean1 = malloc(sizeof(float) * 256);
//     // for (size_t i = 0; i < 256; i++)
//     // {
//     //     mean1[i] = cumsum_mean1[i] / weight1[i];
//     // }
//     // float *mean2 = malloc(sizeof(float) * 256);
//     // for (size_t i = 0, j = 255; i < 256 && j >= 0; i++, j--)
//     // {
//     //     mean2[j] = cumsum_mean2[i] / weight2[j];
//     // }
//     // float *inter_class_variance = malloc(sizeof(float) * 255);
//     // float dnum = 10000000000;
//     // for (size_t i = 0; i < 255; i++)
//     // {
//     //     inter_class_variance[i] = ((weight1[i] * weight2[i] * (mean1[i] - mean2[i + 1])) / dnum) * (mean1[i] - mean2[i + 1]);
//     // }
//     // long double maxi = 0;

//     // int getmax = 0;
//     // for (int i = 0; i < 255; i++)
//     // {
//     //     if (maxi < inter_class_variance[i])
//     //     {
//     //         maxi = inter_class_variance[i];
//     //         getmax = i;
//     //     }
//     // }
//     // printf("%.2f", bin_mids[getmax]);
//     // free(array);
//     // free(bin_edges);
//     // free(bin_mids);
//     // free(weight1);
//     // free(weight2);
//     // free(histogram_bin_mids);
//     // free(cumsum_mean1);
//     // free(cumsum_mean2);
//     // free(mean1);
//     // free(mean2);
//     // free(inter_class_variance);
//     // return bin_mids[getmax];
// }

// SDL_Surface *binarisation(SDL_Surface *image)
// {
//     int w = image->w;
//     int h = image->h;
//     int div = median_image(image);
//     SDL_Surface *image_coppy = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
//     for (int i = 1; i < w - 1; i++)
//     {
//         for (int j = 1; j < h - 1; j++)
//         {

//             Uint8 new_value;
//             if (pixel_grey(image, i, j) <= div)
//             {
//                 new_value = 0;
//             }
//             else
//             {
//                 new_value = 255;
//             }
//             Uint32 pixel_new = SDL_MapRGB(image->format, new_value, new_value, new_value);
//             put_pixel(image_coppy, i, j, pixel_new);
//         }
//     }
//     return image_coppy;
// }

// SDL_Surface *horizontal_edge_detection1(SDL_Surface *image)
// {
//     int w = image->w;
//     int h = image->h;
//     SDL_Surface *image_coppy = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
//     for (int i = 1; i < w - 1; i++)
//     {
//         for (int j = 1; j < h - 1; j++)
//         {
//             //We know hat r = g = b HAVE TO OPTIMISE THAT

//             Uint8 new_value = kernel3x3(
//                 1, 2, 1,
//                 0, 0, 0,
//                 -1, -2, -1,
//                 image, i, j);
//             Uint32 pixel_new = SDL_MapRGB(image->format, new_value, new_value, new_value);
//             put_pixel(image_coppy, i, j, pixel_new);
//         }
//     }
//     trace_beg_end_line(image_coppy, 90, 50, h / 15);
//     return image_coppy;
// }
// SDL_Surface *horizontal_edge_detection(SDL_Surface *image)
// {
//     int w = image->w;
//     int h = image->h;
//     SDL_Surface *image_coppy = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
//     for (int i = 1; i < w - 1; i++)
//     {
//         for (int j = 1; j < h - 1; j++)
//         {
//             //We know hat r = g = b HAVE TO OPTIMISE THAT
//             Uint8 new_value = kernel3x3(
//                 -1, -2, -1,
//                 0, 0, 0,
//                 1, 2, 1,
//                 image, i, j);
//             Uint32 pixel_new = SDL_MapRGB(image->format, new_value, new_value, new_value);
//             put_pixel(image_coppy, i, j, pixel_new);
//         }
//     }
//     trace_beg_end_line(image_coppy, 90, 50, h / 15);
//     return image_coppy;
// }
// SDL_Surface *vertical_edge_detection(SDL_Surface *image)
// {
//     int w = image->w;
//     int h = image->h;
//     SDL_Surface *image_coppy = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
//     for (int i = 1; i < w - 1; i++)
//     {
//         for (int j = 1; j < h - 1; j++)
//         {
//             Uint8 new_value = kernel3x3(
//                 -1, 0, 1,
//                 -2, 0, 2,
//                 -1, 0, 1,
//                 image, i, j);
//             Uint32 pixel_new = SDL_MapRGB(image->format, new_value, new_value, new_value);
//             put_pixel(image_coppy, i, j, pixel_new);
//         }
//     }
//     trace_beg_end_column(image_coppy, 90, 50, w / 15);
//     return image_coppy;
// }

// SDL_Surface *vertical_edge_detection1(SDL_Surface *image)
// {
//     int w = image->w;
//     int h = image->h;
//     SDL_Surface *image_coppy = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
//     for (int i = 1; i < w - 1; i++)
//     {
//         for (int j = 1; j < h - 1; j++)
//         {
//             Uint8 new_value = kernel3x3(
//                 1, 0, -1,
//                 2, 0, -2,
//                 1, 0, -1,
//                 image, i, j);
//             Uint32 pixel_new = SDL_MapRGB(image->format, new_value, new_value, new_value);
//             put_pixel(image_coppy, i, j, pixel_new);
//         }
//     }
//     trace_beg_end_column(image_coppy, 90, 50, w / 15);
//     return image_coppy;
// }

// void run_through_vert(SDL_Surface *image, int h, int w, int tracing_size)
// {
//     int deplacement = 0;
//     int occurances = 0;
//     int current_occurances = 0;
//     int most_green_i = 0;
//     int first_green = 0;
//     int last_green = 0;
//     int save_green_last = 0;
//     int save_green_first = 0;
//     while (tracing_size + deplacement < h)
//     {
//         for (int i = deplacement; i < tracing_size + deplacement; i++)
//         {
//             for (int j = 0; j < w; j++)
//             {
//                 Uint32 pixel = get_pixel(image, j, i);
//                 if (pixel == SDL_MapRGB(image->format, 2, 255, 25))
//                 {
//                     current_occurances += 1;
//                     last_green = j;
//                     if (first_green == 0)
//                     {
//                         first_green = j;
//                     }
//                 }
//             }
//         }
//         if (occurances < current_occurances)
//         {
//             occurances = current_occurances;
//             most_green_i = deplacement;
//             save_green_last = last_green;
//             save_green_first = first_green;
//         }
//         current_occurances = 0;
//         first_green = 0;
//         last_green = 0;
//         deplacement += 1;
//     }
//     // printf("most_green_i: %d first: %d last: %d\n", most_green_i, save_green_first, save_green_last);
//     for (int i = most_green_i; i < tracing_size + most_green_i; i++)
//     {
//         for (int j = save_green_first; j < save_green_last; j++)
//         {
//             Uint32 pixel_new = SDL_MapRGB(image->format, 100, 100, 20);
//             put_pixel(image, j, i, pixel_new);
//         }
//     }
//     SDL_SaveBMP(image, "C:/Users/vladi/SDL_test/src/vert_startline.bmp");
// }

// void run_through_vert1(SDL_Surface *image, int h, int w, int tracing_size)
// {
//     int deplacement = 1;
//     int occurances = 0;
//     int current_occurances = 0;
//     int most_blue_i = 0;
//     int first_blue = 0;
//     int last_blue = 0;
//     int save_blue_last = 0;
//     int save_blue_first = 0;
//     while (tracing_size + deplacement < h)
//     {
//         for (int i = deplacement; i < tracing_size + deplacement; i++)
//         {
//             for (int j = 0; j < w; j++)
//             {
//                 Uint32 pixel = get_pixel(image, j, i);
//                 if (pixel == SDL_MapRGB(image->format, 2, 10, 255))
//                 {
//                     current_occurances += 1;
//                     last_blue = j;
//                     if (first_blue == 0)
//                     {
//                         first_blue = j;
//                     }
//                 }
//             }
//         }
//         if (occurances < current_occurances)
//         {
//             occurances = current_occurances;
//             most_blue_i = deplacement;
//             save_blue_last = last_blue;
//             save_blue_first = first_blue;
//         }
//         current_occurances = 0;
//         first_blue = 0;
//         last_blue = 0;
//         deplacement += 1;
//     }
//     // printf("most_blue_i: %d first: %d last: %d\n", most_blue_i, save_blue_first, save_blue_last);
//     for (int i = most_blue_i; i < tracing_size + most_blue_i; i++)
//     {
//         for (int j = save_blue_first; j < save_blue_last; j++)
//         {
//             Uint32 pixel_new = SDL_MapRGB(image->format, 100, 100, 20);
//             put_pixel(image, j, i, pixel_new);
//         }
//     }
//     SDL_SaveBMP(image, "C:/Users/vladi/SDL_test/src/vert_startline.bmp");
// }
//A motive is something starting with a red pixel followed by n black pixels and ended by a red pixel
//The motive that will be underlined will be the most recurent one

// SDL_Surface *detect_motive_hor(SDL_Surface *image)
// {
//     int h = image->h;
//     int w = image->w;
//     size_t array_size = h / 5;
//     array_type *ar = malloc(sizeof(array_type));
//     ar->len = array_size;
//     ar->array_n = malloc(sizeof(int) * array_size);
//     for (size_t i = 0; i < array_size; i++)
//     {
//         ar->array_n[i] = 0;
//     }
//     for (size_t i = 0; i < image->w; i++)
//     {
//         int j = 0;
//         int serie = 0;
//         Uint32 pixel = get_pixel(image, i, j);

//         //If the pixel seen  not is red
//         while (pixel != SDL_MapRGB(image->format, 255, 0, 0) && j < image->h)
//         {
//             j += 1;
//             pixel = get_pixel(image, i, j);
//         }
//         while (j < image->h)
//         {
//             pixel = get_pixel(image, i, j);
//             Uint8 r, g, b;
//             SDL_GetRGB(pixel, image->format, &r, &g, &b);
//             //scan untill the red ends
//             while (pixel == SDL_MapRGB(image->format, 255, 0, 0) && j < image->h) //while the pixels are red in other words not black
//             {
//                 j += 1;
//                 pixel = get_pixel(image, i, j);
//             }
//             while (pixel != SDL_MapRGB(image->format, 255, 0, 0) && j < image->h) //while the pixels are black in other words not red
//             {
//                 serie += 1;
//                 j += 1;
//                 pixel = get_pixel(image, i, j);
//                 if (serie >= h / 5)
//                 {
//                     serie = 0;
//                     break;
//                 }
//             }
//             //We saw something else than black
//             if (serie > 1)
//             {
//                 ar->array_n[serie - 1] += 1;
//                 serie = 0;
//             }
//             if (j >= image->h)
//             {
//                 break;
//             }
//         }
//     }
//     int max_occ = 0;
//     size_t max_index = 0;
//     for (size_t i = 0; i < array_size; i++)
//     {
//         int val = ar->array_n[i];
//         if (val > 100 && i > 28)
//         {
//             if (max_occ < val)
//             {
//                 max_occ = val;
//                 max_index = i;
//             }
//         }
//     }
//     for (size_t i = 0; i < image->w; i++)
//     {
//         int j = 0;
//         int serie = 0;
//         Uint32 pixel = get_pixel(image, i, j);
//         //If the pixel seen  not is red
//         while (pixel != SDL_MapRGB(image->format, 255, 0, 0) && j < image->h)
//         {
//             j += 1;
//             pixel = get_pixel(image, i, j);
//         }
//         while (j < image->h)
//         {
//             pixel = get_pixel(image, i, j);
//             Uint8 r, g, b;
//             SDL_GetRGB(pixel, image->format, &r, &g, &b);
//             //scan untill the red ends
//             while (pixel == SDL_MapRGB(image->format, 255, 0, 0) && j < image->h) //while the pixels are red in other words not black
//             {
//                 j += 1;
//                 pixel = get_pixel(image, i, j);
//             }
//             while (pixel != SDL_MapRGB(image->format, 255, 0, 0) && j < image->h) //while the pixels are black in other words not red
//             {
//                 serie += 1;
//                 j += 1;
//                 pixel = get_pixel(image, i, j);
//                 if (serie >= h / 5)
//                 {
//                     serie = 0;
//                     break;
//                 }
//             }
//             if (serie >= max_index - (h / 95) && serie <= max_index + (h / 95))
//             {
//                 int actual_j = j - serie;
//                 while (j != actual_j)
//                 {

//                     Uint32 pixel = SDL_MapRGB(image->format, 100, 0, 100);
//                     put_pixel(image, i, actual_j, pixel);
//                     actual_j += 1;
//                 }
//             }
//             //We saw something else than black
//             if (serie > 1)
//             {
//                 ar->array_n[serie - 1] += 1;
//                 serie = 0;
//             }
//             if (j >= image->h)
//             {
//                 break;
//             }
//         }
//     }
//     free(ar);
//     free(ar->array_n);
//     return image;
// }

// SDL_Surface *detect_motive_vert(SDL_Surface *image)
// {
//     int h = image->h;
//     int w = image->w;
//     size_t array_size = w / 5;
//     array_type *ar = malloc(sizeof(array_type));
//     ar->len = array_size;
//     ar->array_n = malloc(sizeof(int) * array_size);
//     for (size_t i = 0; i < array_size; i++)
//     {
//         ar->array_n[i] = 0;
//     }
//     for (size_t i = 0; i < image->h; i++)
//     {
//         int j = 0;
//         int serie = 0;
//         Uint32 pixel = get_pixel(image, j, i);
//         //If the pixel seen  not is red
//         while (pixel != SDL_MapRGB(image->format, 255, 0, 0) && j < image->w)
//         {
//             j += 1;
//             pixel = get_pixel(image, j, i);
//         }
//         while (j < image->w)
//         {
//             pixel = get_pixel(image, j, i);

//             while (pixel == SDL_MapRGB(image->format, 255, 0, 0) && j < image->w) //while the pixels are red in other words not black
//             {
//                 j += 1;
//                 pixel = get_pixel(image, j, i);
//             }
//             while (pixel != SDL_MapRGB(image->format, 255, 0, 0) && j < image->w) //while the pixels are black in other words not red
//             {
//                 serie += 1;
//                 j += 1;
//                 pixel = get_pixel(image, j, i);
//                 if (serie >= w / 5)
//                 {
//                     serie = 0;
//                     break;
//                 }
//             }
//             //We saw something else than black
//             if (serie > 1)
//             {
//                 ar->array_n[serie - 1] += 1;
//                 serie = 0;
//             }
//             if (j >= image->w)
//             {
//                 break;
//             }
//         }
//     }
//     int max_occ = 0;
//     size_t max_index = 0;
//     for (size_t i = 0; i < array_size; i++)
//     {
//         int val = ar->array_n[i];
//         if (val > 100 && i > 28)
//         {
//             if (max_occ < val)
//             {
//                 max_occ = val;
//                 max_index = i;
//             }
//         }
//     }
//     for (size_t i = 0; i < image->h; i++)
//     {
//         int j = 0;
//         int serie = 0;
//         Uint32 pixel = get_pixel(image, j, i);
//         //If the pixel seen  not is red
//         while (pixel != SDL_MapRGB(image->format, 255, 0, 0) && j < image->w)
//         {
//             j += 1;
//             pixel = get_pixel(image, j, i);
//         }
//         while (j < image->w)
//         {
//             pixel = get_pixel(image, j, i);
//             //scan untill the red ends
//             while (pixel == SDL_MapRGB(image->format, 255, 0, 0) && j < image->w) //while the pixels are red in other words not black
//                 pixel = get_pixel(image, ++j, i);

//             while (pixel != SDL_MapRGB(image->format, 255, 0, 0) && j < image->w) //while the pixels are black in other words not red
//             {
//                 pixel = get_pixel(image, ++j, i);
//                 if (++serie >= w / 5)
//                 {
//                     serie = 0;
//                     break;
//                 }
//             }

//             if (serie >= max_index - (w / 95) && serie <= max_index + (w / 95))
//             {
//                 int actual_j = j - serie - 1;
//                 while (j != ++actual_j)
//                     if (get_pixel(image, actual_j, i) == SDL_MapRGB(image->format, 100, 0, 100))
//                         put_pixel(image, actual_j, i, SDL_MapRGB(image->format, 100, 255, 100));
//             }
//             //We saw something else than black
//             if (serie > 1)
//             {
//                 ar->array_n[serie - 1]++;
//                 serie = 0;
//             }
//             if (j >= image->w)
//                 break;
//         }
//     }
//     free(ar);
//     free(ar->array_n);
//     return image;
// }

// void combine_detections(SDL_Surface *image, SDL_Surface *image1)
// {
//     for (size_t i = 0; i < image->w; i++)
//         for (size_t j = 0; j < image->h; j++)
//             if (get_pixel(image, i, j) == SDL_MapRGB(image->format, 100, 0, 100))
//                 if (get_pixel(image1, i, j) == SDL_MapRGB(image1->format, 0, 255, 0))
//                     put_pixel(image, i, j, SDL_MapRGB(image->format, 100, 255, 100));
//     SDL_SaveBMP(image, "C:/Users/vladi/SDL_test/src/detect_all.bmp");
// }

// //Calling the function knowing we have just detected greeeeeeeeen
// //It counts the number of greeeeeeeeen occurances in a hor line
// size_t detect_green_hor(SDL_Surface *image, size_t x, size_t y)
// {
//     size_t count = 0;
//     Uint32 pixel;
//     while ((pixel = get_pixel(image, x++, y)) == SDL_MapRGB(image->format, 100, 255, 100))
//         count++;
//     return count;
// }

// //Calling the function knowing we have just detected greeeeeeeeen
// //It counts the number of greeeeeeeeen occurances in a vert line
// size_t detect_green_vert(SDL_Surface *image, size_t x, size_t y)
// {
//     size_t count = 0;
//     Uint32 pixel;
//     while ((pixel = get_pixel(image, x, y++)) == SDL_MapRGB(image->format, 100, 255, 100))
//         count++;
//     return count;
// }

// char *create_str(int number)
// {
//     char str_path[] = "C:/Users/vladi/SDL_test/src/num";
//     size_t str_path_len = 0;
//     while (str_path[str_path_len] != 0)
//         str_path_len++;

//     char str_bmp[] = ".bmp";
//     size_t str_bmp_len = 0;

//     while (str_bmp[str_bmp_len] != 0)
//         str_bmp_len++;

//     int rem_num = number;
//     size_t count = 0;
//     while (number != 0)
//     {
//         count++;
//         number /= 10;
//     }

//     char *str = malloc((count + 1) * sizeof(char));
//     str[count] = '\0';

//     for (size_t i = count; i > 0; i--)
//     {
//         str[i - 1] = (rem_num - (rem_num / 10) * 10) + '0';
//         rem_num /= 10;
//     }

//     size_t str_path_malloc_len = str_path_len + count + str_bmp_len;
//     char *str_path_malloc = malloc((str_path_len + count + str_bmp_len) * sizeof(char));
//     str_path_malloc[str_path_malloc_len] = '\0';

//     for (size_t i = str_bmp_len; i > 0; i--)
//         str_path_malloc[--str_path_malloc_len] = str_bmp[i - 1];

//     for (size_t i = count; i > 0; i--)
//         str_path_malloc[--str_path_malloc_len] = str[i - 1];

//     free(str);

//     for (size_t i = str_path_len; i > 0; i--)
//         str_path_malloc[--str_path_malloc_len] = str_path[i - 1];

//     return str_path_malloc;
// }
// void detect_green(SDL_Surface *image_green, SDL_Surface *original)
// {
//     int h = image_green->h;
//     int w = image_green->w;
//     // Count_numbers represents the case number
//     int count_numbers = 0;
//     size_t new_i = 0;
//     size_t j = 0;
//     while (j < h)
//     {
//         size_t i = new_i;
//         while (i < w)
//         {
//             // Uint32 pixel_d = SDL_MapRGB(original->format,28,j*0.1,j*0.1);
//             // put_pixel(original,i,j,pixel_d);
//             //Get the pixel at i,j
//             Uint32 pixel = get_pixel(image_green, i, j);
//             //If it is not a detected pixel, increment j
//             //else if it is a detected pixel
//             if (pixel == SDL_MapRGB(image_green->format, 100, 255, 100))
//             {
//                 //Find the hight from the green we detected
//                 size_t new_detected_h = detect_green_vert(image_green, i, j);
//                 //Find the width from the green we detected
//                 size_t new_detected_w = detect_green_hor(image_green, i, j);
//                 //verify that this could be a cube
//                 //If it can't be one increment j
//                 //To look at the next pixel and go
//                 //To the next loop

//                 double divh_w = (double)new_detected_h / (double)new_detected_w;
//                 double divw_h = (double)new_detected_w / (double)new_detected_h;

//                 if ((divh_w < 0.85 || divw_h < 0.85) || ((double)new_detected_h < (h/33.) || (double)new_detected_w < (w/33.)))
//                 {

//                     i += 1;
//                     continue;
//                 }
//                 printf(" number %d -> corner i %zu, j %zu\n", count_numbers, i, j);
//                 printf("h/w -> %f , w/h -> %f, new_h -> %zu , new_w -> %zu\n", divh_w, divw_h, new_detected_h, new_detected_w);
//                 printf("%d\n", ((double)new_detected_h < 40. || (double)new_detected_w < 40.));

//                 //Else it looks like a cube structure
//                 //Create a placement for it
//                 SDL_Surface *number = SDL_CreateRGBSurface(0, new_detected_w, new_detected_h, 32, 0, 0, 0, 0);
//                 //Set an index indicating the end of the sequence vertically
//                 size_t index_j = new_detected_h + j;
//                 //Set an index indicating the end of the sequence horizontally
//                 size_t index_i = new_detected_w + i;
//                 // Memorise both current i and j
//                 size_t remeber_i = i;
//                 size_t remebre_j = j;
//                 //Initialise the index counters for the sequence at 0
//                 size_t num_i = 0;
//                 size_t num_j = 0;
//                 //Start looping throught the sequence
//                 while (j < index_j)
//                 {
//                     //Start looping through the sequence
//                     while (i < index_i)
//                     {
//                         //Get the pixel from the original image
//                         pixel = get_pixel(original, i, j);
//                         // Put the pixel at it's placement in the
//                         //number SDL_Surface
//                         put_pixel(number, num_i, num_j, pixel);
//                         //At the same time mark with green the place
//                         // From which you have extracted the pixel
//                         //That was put into number
//                         Uint32 pixel1 = SDL_MapRGB(image_green->format, 0, 0, 255);
//                         put_pixel(original, i, j, pixel1);
//                         put_pixel(image_green, i, j, pixel1);
//                         // increment both i and num_i indexes
//                         i += 1;
//                         num_i += 1;
//                     }
//                     //A line has been ran through
//                     //So num_i is put to zero
//                     num_i = 0;
//                     //And i is restored
//                     i = remeber_i;
//                     // Increment j
//                     j += 1;
//                     num_j += 1;
//                 }
//                 char *str = create_str(count_numbers);
//                 SDL_SaveBMP(number, str);
//                 free(str);
//                 SDL_FreeSurface(number);

//                 count_numbers += 1;

//                 if (count_numbers + 1 % 9 == 0)
//                 {
//                     i = 0;
//                     j = index_j;
//                 }
//                 if (count_numbers + 1 % 9 != 0)
//                 {
//                     i = index_i;
//                     j = remebre_j;
//                 }
//                 pixel = get_pixel(image_green, i, j);
//             }
//             i += 1;
//         }
//         j += 1;
//     }
// // }
// SDL_Surface *hough_alligned(SDL_Surface *image)
// {
//     int w = image->w;
//     int h = image->h;
//     int max_dist = (int)(round(sqrt(w * w + h * h)));
//     printf("max_dist -> %d\n", max_dist);
//     // SDL_Surface *hough = SDL_CreateRGBSurface(0, 2 * max_dist, 181, 32, 0, 0, 0, 0);
//     int parcours = 2 * max_dist * 181;
//     Couple *couple_list = malloc(parcours * sizeof(Couple));
//     double *hough_tr = malloc(sizeof(double) * parcours);
//     //max_dist la diagonale de l'image
//     for (size_t i = 0; i < parcours; i++)
//     {
//         hough_tr[i] = 0;
//     }

//     //degree values from -90 to 90
//     double *thetas = malloc(sizeof(double) * 181);
//     size_t j = 0;
//     for (double i = 0; i < 181; i++)
//     {
//         double values = i * M_PI / 180;
//         thetas[j] = values;
//         j++;
//     }
//     int biggest_pix = 0;
//     for (double y = 10; y < h - 10; y++)
//     {
//         for (double x = 10; x < w - 10; x++)
//         {
//             if (pixel_grey(image, x, y) < 200)
//             {
//                 for (size_t k = 0; k < 181; k++)
//                 {
//                     double current_theta = thetas[k];
//                     double ro = x * cos(current_theta) + y * sin(current_theta);
//                     int index_hough = k * 2 * max_dist + (int)ro + max_dist;
//                     double new_pix = ++hough_tr[index_hough];
//                     couple_list[index_hough].occurence = new_pix;
//                     couple_list[index_hough].ro = ro;
//                     couple_list[index_hough].theta = current_theta;
//                     if (biggest_pix < new_pix)
//                     {
//                         biggest_pix = new_pix;
//                     }
//                 }
//             }
//         }
//     }
//     free(thetas);
//     int bornes = max_dist / 3.5;
//     int couple_number = 0;
//     for (size_t i = 0; i < 2 * max_dist * 181; i++)
//     {

//         if (couple_list[i].occurence >= biggest_pix - bornes)
//         {
//             couple_number++;
//         }
//     }
//     Couple *reduced_couple_list = malloc(sizeof(Couple) * couple_number);
//     int theta_numbers = 0;
//     double new_theta = 0.;
//     size_t z = 0;
//     for (size_t i = 0; i < 2 * max_dist * 181; i++)
//     {
//         if (couple_list[i].occurence >= biggest_pix - bornes)
//         {
//             reduced_couple_list[z] = couple_list[i];
//             printf("occurence -> %d, theta -> %f, ro -> %f\n",
//                    reduced_couple_list[z].occurence, reduced_couple_list[z].theta, reduced_couple_list[z].ro);

//             if (new_theta != reduced_couple_list[z].theta)
//             {
//                 new_theta = reduced_couple_list[z].theta;

//                 theta_numbers += 1.;
//             }
//             if (reduced_couple_list[z].theta < 1.58 && reduced_couple_list[z].theta > 1.55)
//             {
//                 for (double x = 10; x < w - 10; x++)
//                 {
//                     double y = (-cos(reduced_couple_list[z].theta) / sin(reduced_couple_list[z].theta)) * x + (reduced_couple_list[z].ro / sin(reduced_couple_list[z].theta));
//                     Uint32 pix = SDL_MapRGB(image->format, 255, 0, 0);
//                     if (y > 0. && y < h)
//                     {

//                         if (get_pixel(image, x, y) == SDL_MapRGB(image->format, 0, 0, 0))
//                         {
//                             put_pixel(image, (int)x, (int)y, pix);
//                         }
//                     }
//                 }
//             }
//             z++;
//         }
//     }
//     free(reduced_couple_list);
//     free(couple_list);
//     free(hough_tr);
//     return image;
// }

// Rotated *hough_transform(SDL_Surface *image, SDL_Surface *original)
// {
//     int w = image->w;
//     int h = image->h;
//     int max_dist = (int)(round(sqrt(w * w + h * h)));
//     printf("max_dist -> %d\n", max_dist);
//     // SDL_Surface *hough = SDL_CreateRGBSurface(0, 2 * max_dist, 181, 32, 0, 0, 0, 0);
//     int parcours = 2 * max_dist * 181;
//     Rotated *image_theta = malloc(sizeof(Rotated));
//     Couple *couple_list = malloc(parcours * sizeof(Couple));
//     double *hough_tr = malloc(sizeof(double) * parcours);
//     //max_dist la diagonale de l'image
//     for (size_t i = 0; i < parcours; i++)
//     {
//         hough_tr[i] = 0;
//     }

//     //degree values from -90 to 90
//     double *thetas = malloc(sizeof(double) * 181);
//     size_t j = 0;
//     for (double i = 0; i < 181; i++)
//     {
//         double values = i * M_PI / 180;
//         thetas[j] = values;
//         j++;
//     }
//     int biggest_pix = 0;
//     for (double y = 10; y < h - 10; y++)
//     {
//         for (double x = 10; x < w - 10; x++)
//         {
//             if (pixel_grey(image, x, y) < 200)
//             {
//                 for (size_t k = 0; k < 181; k++)
//                 {
//                     double current_theta = thetas[k];
//                     double ro = x * cos(current_theta) + y * sin(current_theta);
//                     int index_hough = k * 2 * max_dist + (int)ro + max_dist;
//                     double new_pix = ++hough_tr[index_hough];
//                     couple_list[index_hough].occurence = new_pix;
//                     couple_list[index_hough].ro = ro;
//                     couple_list[index_hough].theta = current_theta;
//                     if (biggest_pix < new_pix)
//                     {
//                         biggest_pix = new_pix;
//                     }
//                 }
//             }
//         }
//     }
//     free(thetas);
//     int bornes = max_dist / 4;
//     int couple_number = 0;
//     for (size_t i = 0; i < 2 * max_dist * 181; i++)
//     {

//         if (couple_list[i].occurence >= biggest_pix - bornes)
//         {
//             couple_number++;
//         }
//     }
//     Couple *reduced_couple_list = malloc(sizeof(Couple) * couple_number);
//     int theta_numbers = 0;
//     double new_theta = 0.;
//     size_t z = 0;
//     for (size_t i = 0; i < 2 * max_dist * 181; i++)
//     {
//         if (couple_list[i].occurence >= biggest_pix - bornes)
//         {
//             reduced_couple_list[z] = couple_list[i];
//             printf("occurence -> %d, theta -> %f, ro -> %f\n",
//                    reduced_couple_list[z].occurence, reduced_couple_list[z].theta, reduced_couple_list[z].ro);

//             if (new_theta != reduced_couple_list[z].theta)
//             {
//                 new_theta = reduced_couple_list[z].theta;

//                 theta_numbers += 1.;
//             }
//             // if (reduced_couple_list[z].theta < 1.6 && reduced_couple_list[z].theta > 1.4 )
//             // {

//             // for (double x = 10; x < w - 10; x++)
//             // {
//             //     double y = (-cos(reduced_couple_list[z].theta) / sin(reduced_couple_list[z].theta)) * x + (reduced_couple_list[z].ro / sin(reduced_couple_list[z].theta));
//             //     Uint32 pix = SDL_MapRGB(image->format, 255, 0, 0);
//             //     if (y > 0. && y < h)
//             //     {

//             //         put_pixel(image, (int)x, (int)y, pix);
//             //     }
//             // }
//             // }
//             z++;
//         }
//     }
//     // SDL_SaveBMP(image, "C:/Users/vladi/SDL_test/src/hough_all.bmp");
//     printf("theta_numbers ->%d\n", theta_numbers);
//     Couple *grouped = malloc(sizeof(Couple) * (theta_numbers + 1));
//     for (size_t i = 0; i < theta_numbers; i++)
//     {
//         grouped[i].ro = 0.;
//         grouped[i].occurence = 0;
//         grouped[i].theta = 0.;
//     }

//     double theta_new = 10.;
//     double nomber_occ = 0.;
//     int biggest_occurence = 0;
//     int current_occurence = 0;
//     size_t biggest_theta_occurence = 0;
//     size_t ind = -1;
//     printf("z -> %zu", z);
//     for (size_t i = 0; i < z; i++)
//     {
//         if (reduced_couple_list[i].theta != theta_new)
//         {
//             if (biggest_occurence < current_occurence)
//             {
//                 biggest_occurence = current_occurence;
//                 biggest_theta_occurence = ind;
//             }
//             current_occurence = 0;
//             ind++;
//             theta_new = reduced_couple_list[i].theta;
//         }
//         else
//         {
//             grouped[ind].occurence += reduced_couple_list[i].occurence;
//             current_occurence = grouped[ind].occurence;
//             grouped[ind].theta = reduced_couple_list[i].theta;
//             grouped[ind].ro = reduced_couple_list[i].ro;
//         }
//     }
//     for (size_t i = 0; i < theta_numbers; i++)
//     {
//         printf("occurence -> %d, theta -> %f, ro -> %f\n",
//                grouped[i].occurence, grouped[i].theta, grouped[i].ro);
//     }
//     printf("biggest_occurence -> %d\n", biggest_occurence);
//     printf("final theta -> %f\n", grouped[biggest_theta_occurence]);
//     if (grouped[biggest_theta_occurence].theta < 1.58 && grouped[biggest_theta_occurence].theta > 1.55 || grouped[biggest_theta_occurence].theta == 0)
//     {
//         printf("The sudoku is aligned\n");
//         for (size_t i = 0; i < z; i++)
//         {
//             if (reduced_couple_list[i].theta < 1.58 && reduced_couple_list[i].theta > 1.55)
//             {
//                 for (double x = 10; x < w - 10; x++)
//                 {
//                     double y = (-cos(reduced_couple_list[i].theta) / sin(reduced_couple_list[i].theta)) * x + (reduced_couple_list[i].ro / sin(reduced_couple_list[i].theta));
//                     Uint32 pix = SDL_MapRGB(image->format, 255, 0, 0);
//                     if (y > 0. && y < h)
//                     {
//                         if (get_pixel(image, x, y) == SDL_MapRGB(image->format, 0, 0, 0))
//                         {
//                             put_pixel(image, (int)x, (int)y, pix);
//                         }
//                     }
//                 }
//             }
//         }
//         image = rotation_90(image);
//         SDL_SaveBMP(image, "C:/Users/vladi/SDL_test/src/rotate90.bmp");
//         image = hough_alligned(image);
//         image = rotation_270(image);
//         image_theta->image_output = image;
//         image_theta->theta = -1;
//     }
//     else
//     {
//         for (size_t i = 0; i < z; i++)
//         {

//             for (double x = 10; x < w - 10; x++)
//             {
//                 double y = (-cos(reduced_couple_list[i].theta) / sin(reduced_couple_list[i].theta)) * x + (reduced_couple_list[i].ro / sin(reduced_couple_list[i].theta));
//                 Uint32 pix = SDL_MapRGB(image->format, 255, 0, 0);
//                 if (y > 0. && y < h)
//                 {
//                     if (get_pixel(image, x, y) == SDL_MapRGB(image->format, 0, 0, 0))
//                     {
//                         put_pixel(image, (int)x, (int)y, pix);
//                     }
//                 }
//             }
//         }
//         image = rotate(image, grouped[biggest_theta_occurence-2].theta);
//         image_theta->image_output = image;
//         image_theta->theta = grouped[biggest_theta_occurence-2].theta;
//     }
//     free(grouped);
//     free(reduced_couple_list);
//     free(couple_list);
//     free(hough_tr);
//     return image_theta;
// // }
// //This function will clean up the image after detect_green
// //Getting rid of purple pixels adjacent to greens,
// //As well as this strips of white and red pixels
// SDL_Surface *clean_up(SDL_Surface *image)
// {
//     int h = image->h;
//     int w = image->w;
//     Uint32 green_pix = SDL_MapRGB(image->format, 100, 255, 100);
//     for (size_t i = 1; i < w - 1; i++)
//     {
//         for (size_t j = 1; j < h - 1; j++)
//         {
//             Uint32 pix_to_test = get_pixel(image, i, j);
//             if (pix_to_test == SDL_MapRGB(image->format, 100, 0, 100) ||
//                 // pix_to_test == SDL_MapRGB(image->format, 255, 255, 255) ||
//                 pix_to_test == SDL_MapRGB(image->format, 0, 255, 0) ||
//                 pix_to_test == SDL_MapRGB(image->format, 0, 0, 0))
//             {
//                 if (get_pixel(image, i, j - 1) == green_pix
//                 || get_pixel(image, i, j + 1) == green_pix 
//                 || get_pixel(image, i - 1, j) == green_pix 
//                 || get_pixel(image, i + 1, j) == green_pix )
//                 {
//                     put_pixel(image, i, j, green_pix);
//                 }
//             }
//             if (pix_to_test == SDL_MapRGB(image->format, 255, 255, 255))
//             {
//                 if (
//                     get_pixel(image, i + 1, j) == green_pix && get_pixel(image, i - 1, j) == green_pix)
//                 {
//                     put_pixel(image, i, j, green_pix);
//                 }
//             }
            
//         }
//     }
//     return image;
// }



int main(int argc, char *argv[])
{
    int quit = 0;
    SDL_Event event;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Sudoku",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1300, 1300, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Surface *image = SDL_LoadBMP(argv[1]);
    //GreyScale for image original
    grey_scale(image);
    SDL_Surface *after_gaussian_blur = gaussian_blur(image);
    SDL_SaveBMP(after_gaussian_blur, "C:/Users/vladi/SDL_test/src/gauss.bmp");
    SDL_Surface *after_median = median5(after_gaussian_blur);
    SDL_SaveBMP(after_median, "C:/Users/vladi/SDL_test/src/median.bmp");
    after_median = sobel(after_median);
    SDL_SaveBMP(after_median, "C:/Users/vladi/SDL_test/src/sobel.bmp");
    int threshold = Otsu(after_median);
    OtsuBinarization(after_median, threshold);
    after_median = hysteris(after_median);
    SDL_SaveBMP(after_median, "C:/Users/vladi/SDL_test/src/hysteris.bmp");
    Rotated *image_theta = hough_transform(after_median,after_gaussian_blur);
    image = image_theta->image_output;
    SDL_SaveBMP(image_theta->image_output, "C:/Users/vladi/SDL_test/src/hough.bmp");
    after_gaussian_blur= rotate(after_gaussian_blur,image_theta->theta);
    SDL_Surface *image_hor = detect_motive_hor(image);
    SDL_SaveBMP(image_hor, "C:/Users/vladi/SDL_test/src/hor.bmp");
    SDL_Surface *image_vert = detect_motive_vert(image);
    SDL_SaveBMP(image_vert, "C:/Users/vladi/SDL_test/src/vert.bmp");
    image = clean_up(image_vert);
    SDL_SaveBMP(image_vert, "C:/Users/vladi/SDL_test/src/clean_up.bmp");
    detect_green(image_vert, after_gaussian_blur,argv[2]);
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
    SDL_FreeSurface(after_median);
    SDL_FreeSurface(after_gaussian_blur);
    SDL_FreeSurface(image_hor);
    SDL_FreeSurface(image_vert);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}