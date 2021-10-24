#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "basic_pixel.h"
#include <math.h>
#include "rotation.h"

const double  M_PI =  3.14159265358979323846;
typedef struct Couple
{
    double theta;
    double ro;
    int occurence;
} Couple;

typedef struct Rotated
{
    SDL_Surface *image_output;
    double theta;
} Rotated;

SDL_Surface *hough_alligned(SDL_Surface *image)
{
    int w = image->w;
    int h = image->h;
    int max_dist = (int)(round(sqrt(w * w + h * h)));
    int parcours = 2 * max_dist * 181;
    Couple *couple_list = malloc(parcours * sizeof(Couple));
    double *hough_tr = malloc(sizeof(double) * parcours);
    //max_dist la diagonale de l'image
    for (int i = 0; i < parcours; i++)
    {
        hough_tr[i] = 0;
    }

    //degree values from -90 to 90
    double *thetas = malloc(sizeof(double) * 181);
    size_t j = 0;
    for (double i = 0; i < 181; i++)
    {
        double values = i * M_PI / 180;
        thetas[j] = values;
        j++;
    }
    int biggest_pix = 0;
    for (double y = 10; y < h - 10; y++)
    {
        for (double x = 10; x < w - 10; x++)
        {
            if (pixel_grey(image, x, y) < 200)
            {
                for (size_t k = 0; k < 181; k++)
                {
                    double current_theta = thetas[k];
                    double ro = x * cos(current_theta) + y * sin(current_theta);
                    int index_hough = k * 2 * max_dist + (int)ro + max_dist;
                    double new_pix = ++hough_tr[index_hough];
                    couple_list[index_hough].occurence = new_pix;
                    couple_list[index_hough].ro = ro;
                    couple_list[index_hough].theta = current_theta;
                    if (biggest_pix < new_pix)
                    {
                        biggest_pix = new_pix;
                    }
                }
            }
        }
    }
    free(thetas);
    int bornes = max_dist / 3.5;
    int couple_number = 0;
    for (int i = 0; i < parcours; i++)
    {

        if (couple_list[i].occurence >= biggest_pix - bornes)
        {
            couple_number++;
        }
    }
    Couple *reduced_couple_list = malloc(sizeof(Couple) * couple_number);
    int theta_numbers = 0;
    double new_theta = 0.;
    size_t z = 0;
    for (int i = 0; i < parcours; i++)
    {
        if (couple_list[i].occurence >= biggest_pix - bornes)
        {
            reduced_couple_list[z] = couple_list[i];
            printf("occurence -> %d, theta -> %f, ro -> %f\n",
                   reduced_couple_list[z].occurence, reduced_couple_list[z].theta, reduced_couple_list[z].ro);

            if (new_theta != reduced_couple_list[z].theta)
            {
                new_theta = reduced_couple_list[z].theta;

                theta_numbers += 1.;
            }
            if (reduced_couple_list[z].theta < 1.58 && reduced_couple_list[z].theta > 1.55)
            {
                for (double x = 10; x < w - 10; x++)
                {
                    double y = (-cos(reduced_couple_list[z].theta) / sin(reduced_couple_list[z].theta)) * x + (reduced_couple_list[z].ro / sin(reduced_couple_list[z].theta));
                    Uint32 pix = SDL_MapRGB(image->format, 255, 0, 0);
                    if (y > 0. && y < h)
                    {

                        if (get_pixel(image, x, y) == SDL_MapRGB(image->format, 0, 0, 0))
                        {
                            put_pixel(image, (int)x, (int)y, pix);
                        }
                    }
                }
            }
            z++;
        }
    }
    free(reduced_couple_list);
    free(couple_list);
    free(hough_tr);
    return image;
}

Rotated *hough_transform(SDL_Surface *image)
{
    int w = image->w;
    int h = image->h;
    int max_dist = (int)(round(sqrt(w * w + h * h)));
    printf("max_dist -> %d\n", max_dist);
    // SDL_Surface *hough = SDL_CreateRGBSurface(0, 2 * max_dist, 181, 32, 0, 0, 0, 0);
    size_t parcours = 2 * max_dist * 181;
    Rotated *image_theta = malloc(sizeof(Rotated));
    Couple *couple_list = malloc(parcours * sizeof(Couple));
    double *hough_tr = malloc(sizeof(double) * parcours);
    //max_dist la diagonale de l'image
    for (size_t i = 0; i < parcours; i++)
    {
        hough_tr[i] = 0;
    }

    //degree values from -90 to 90
    double *thetas = malloc(sizeof(double) * 181);
    size_t j = 0;
    for (double i = 0; i < 181; i++)
    {
        double values = i * M_PI / 180;
        thetas[j] = values;
        j++;
    }
    int biggest_pix = 0;
    for (double y = 10; y < h - 10; y++)
    {
        for (double x = 10; x < w - 10; x++)
        {
            if (pixel_grey(image, x, y) < 200)
            {
                for (size_t k = 0; k < 181; k++)
                {
                    double current_theta = thetas[k];
                    double ro = x * cos(current_theta) + y * sin(current_theta);
                    int index_hough = k * 2 * max_dist + (int)ro + max_dist;
                    double new_pix = ++hough_tr[index_hough];
                    couple_list[index_hough].occurence = new_pix;
                    couple_list[index_hough].ro = ro;
                    couple_list[index_hough].theta = current_theta;
                    if (biggest_pix < new_pix)
                    {
                        biggest_pix = new_pix;
                    }
                }
            }
        }
    }
    free(thetas);
    int bornes = max_dist / 4;
    int couple_number = 0;
    for (size_t i = 0; i < parcours; i++)
    {

        if (couple_list[i].occurence >= biggest_pix - bornes)
        {
            couple_number++;
        }
    }
    Couple *reduced_couple_list = malloc(sizeof(Couple) * couple_number);
    int theta_numbers = 0;
    double new_theta = 0.;
    size_t z = 0;
    for (size_t i = 0; i < parcours; i++)
    {
        if (couple_list[i].occurence >= biggest_pix - bornes)
        {
            reduced_couple_list[z] = couple_list[i];
            printf("occurence -> %d, theta -> %f, ro -> %f\n",
                   reduced_couple_list[z].occurence, reduced_couple_list[z].theta, reduced_couple_list[z].ro);

            if (new_theta != reduced_couple_list[z].theta)
            {
                new_theta = reduced_couple_list[z].theta;

                theta_numbers += 1.;
            }
            // if (reduced_couple_list[z].theta < 1.6 && reduced_couple_list[z].theta > 1.4 )
            // {

            // for (double x = 10; x < w - 10; x++)
            // {
            //     double y = (-cos(reduced_couple_list[z].theta) / sin(reduced_couple_list[z].theta)) * x + (reduced_couple_list[z].ro / sin(reduced_couple_list[z].theta));
            //     Uint32 pix = SDL_MapRGB(image->format, 255, 0, 0);
            //     if (y > 0. && y < h)
            //     {

            //         put_pixel(image, (int)x, (int)y, pix);
            //     }
            // }
            // }
            z++;
        }
    }
    // SDL_SaveBMP(image, "C:/Users/vladi/SDL_test/src/hough_all.bmp");
    printf("theta_numbers ->%d\n", theta_numbers);
    Couple *grouped = malloc(sizeof(Couple) * (theta_numbers + 1));
    for (int i = 0; i < theta_numbers; i++)
    {
        grouped[i].ro = 0.;
        grouped[i].occurence = 0;
        grouped[i].theta = 0.;
    }

    double theta_new = 10.;
    int biggest_occurence = 0;
    int current_occurence = 0;
    size_t biggest_theta_occurence = 0;
    size_t ind = -1;
    printf("z -> %zu", z);
    for (size_t i = 0; i < z; i++)
    {
        if (reduced_couple_list[i].theta != theta_new)
        {
            if (biggest_occurence < current_occurence)
            {
                biggest_occurence = current_occurence;
                biggest_theta_occurence = ind;
            }
            current_occurence = 0;
            ind++;
            theta_new = reduced_couple_list[i].theta;
        }
        else
        {
            grouped[ind].occurence += reduced_couple_list[i].occurence;
            current_occurence = grouped[ind].occurence;
            grouped[ind].theta = reduced_couple_list[i].theta;
            grouped[ind].ro = reduced_couple_list[i].ro;
        }
    }
    for (int i = 0; i < theta_numbers; i++)
    {
        printf("occurence -> %d, theta -> %f, ro -> %f\n",
               grouped[i].occurence, grouped[i].theta, grouped[i].ro);
    }
    printf("biggest_occurence -> %d\n", biggest_occurence);
    printf("final theta -> %f\n", grouped[biggest_theta_occurence].theta);
    if ((grouped[biggest_theta_occurence].theta < 1.58 && grouped[biggest_theta_occurence].theta > 1.55) || grouped[biggest_theta_occurence].theta == 0)
    {
        printf("The sudoku is aligned\n");
        for (size_t i = 0; i < z; i++)
        {
            if (reduced_couple_list[i].theta < 1.58 && reduced_couple_list[i].theta > 1.55)
            {
                for (double x = 10; x < w - 10; x++)
                {
                    double y = (-cos(reduced_couple_list[i].theta) / sin(reduced_couple_list[i].theta)) * x + (reduced_couple_list[i].ro / sin(reduced_couple_list[i].theta));
                    Uint32 pix = SDL_MapRGB(image->format, 255, 0, 0);
                    if (y > 0. && y < h)
                    {
                        if (get_pixel(image, x, y) == SDL_MapRGB(image->format, 0, 0, 0))
                        {
                            put_pixel(image, (int)x, (int)y, pix);
                        }
                    }
                }
            }
        }
	printf("We are through\n");
        image = rotation_90(image);
	printf("We are through 90\n");
        image = hough_alligned(image);
	printf("We are through hough_alligned\n");
        image = rotation_270(image);
	printf("We are through 270\n");
        image_theta->image_output = image;
	printf("Image passed \n");
        image_theta->theta = -1;
	printf("Theta init\n");
    }
    else
    {
        for (size_t i = 0; i < z; i++)
        {

            for (double x = 10; x < w - 10; x++)
            {
		    printf("In the Else occurence -> %d, theta -> %f, ro -> %f\n", reduced_couple_list[i].occurence, reduced_couple_list[i].theta, reduced_couple_list[i].ro); 
                double y = (-cos(reduced_couple_list[i].theta) / sin(reduced_couple_list[i].theta)) * x + (reduced_couple_list[i].ro / sin(reduced_couple_list[i].theta));
                Uint32 pix = SDL_MapRGB(image->format, 255, 0, 0);
                if (y > 0. && y < h)
                {
                    if (get_pixel(image, x, y) == SDL_MapRGB(image->format, 0, 0, 0))
                    {
                        put_pixel(image, (int)x, (int)y, pix);
                    }
                }
            }
        }
	printf("We scanned through\n");
        image_theta->image_output  = rotate(image, grouped[biggest_theta_occurence-2].theta);
	printf("We passed rotation");
        image_theta->theta = grouped[biggest_theta_occurence-2].theta;
	printf("theta -> %f\n",image_theta->theta);
    }
    free(grouped);
    free(reduced_couple_list);
    free(couple_list);
    free(hough_tr);
    return image_theta;
}
