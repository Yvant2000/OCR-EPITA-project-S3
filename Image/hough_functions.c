#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "basic_pixel.h"
#include <math.h>
#include "rotation.h"

const double  M_PI =  3.14159265358979323846;



typedef struct Triple
{
    double theta;
    double ro;
    int occurence;
    int x;
    int y;
} Triple;

typedef struct Rotated
{
    SDL_Surface *image_output;
    double theta;
} Rotated;

SDL_Surface *trim_green(SDL_Surface *image, SDL_Surface *cleaned)
{
    size_t h = image->h;
    size_t w = image->w;
    size_t bottom_trim = 0;
    size_t top_trim = 0;
    size_t right_trim = 0;
    size_t left_trim = 0;
    int done = 0;
    for (size_t j = h-1; j > 0; j--)
    {
        for (size_t i = 0; i < w; i++)
        { //printf("bottom i -> %zu , j -> %zu\n",i,j);
            Uint32 pix = get_pixel(image, i, j);
            //If the pixel is not black
            if (pix == SDL_MapRGB(image->format, 0, 255, 0))
            {
                //trim here on the bottom
                bottom_trim = j+1;
                done = 1;
                break;
            }
        }
        if(done)
        {
            break;
        }
    }
    done = 0;
    for (size_t j = 0; j < h; j++)
    {
        for (size_t i = 0; i < w; i++)
        { //printf("top i -> %zu , j -> %zu\n",i,j);
            Uint32 pix = get_pixel(image, i, j);
            //If the pixel is not black
            if (pix == SDL_MapRGB(image->format, 0, 255, 0))
            {
                //trim here on the top
                top_trim = j-1;
                done = 1;
                break;
            }
        }
        if(done)
        {
            break;
        }
    }
    done = 0;
    for (size_t i = 0; i < w; i++)
    {
        for (size_t j = top_trim; j < bottom_trim; j++)
        { //printf("right i -> %zu , j -> %zu\n",i,j);
            Uint32 pix = get_pixel(image, i, j);
            //If the pixel is not black
            if (pix == SDL_MapRGB(image->format, 0, 255, 0))
            {
                //trim here on the right
                left_trim =  i-1;
                done = 1;
                break;
            }
        }
        if(done)
        {
            break;
        }
    }
    done = 0;
    for (size_t i = w-1; i > 0; i--)
    {
        for (size_t j = top_trim; j < bottom_trim; j++)
        { //printf("left i -> %zu , j -> %zu\n",i,j);
            Uint32 pix = get_pixel(image, i, j);
            //If the pixel is not black
            if (pix == SDL_MapRGB(image->format, 0, 255, 0))
            {
                //trim here on the left
                right_trim =  i+1;
                done = 1;
                break;
            }
        }
        if(done)
        {
            break;
        }
    }
    printf("bot -> %zu\n",bottom_trim);
    printf("top -> %zu\n",top_trim);
    printf("left -> %zu\n",left_trim);
    printf("right -> %zu\n",right_trim);
    SDL_Surface *output_image = SDL_CreateRGBSurface(0, (int)(right_trim - left_trim),(int)(bottom_trim - top_trim), 32, 0,0,0,0);
    printf("new_width -> %d\n",(int)(right_trim - left_trim));
    printf("new_height -> %d\n", (int)(bottom_trim - top_trim));
    for (size_t i = left_trim; i < right_trim; i++)
    {
        for (size_t j = top_trim; j < bottom_trim; j++)
        {
            Uint32 pix = get_pixel(cleaned, i, j);
            put_pixel(output_image,(int)(i-left_trim),(int)(j-top_trim),pix);
        }
    }
    SDL_FreeSurface(image);
    return output_image;
}

SDL_Surface *hough_alligned(SDL_Surface *image,double thresh)
{
    int w = image->w;
    int h = image->h;
    int max_dist = (int)(round(sqrt(w * w + h * h)));
    int parcours = 2 * max_dist * 181;
    Triple *couple_list = malloc(parcours * sizeof(Triple));
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
    for (double y = 7; y < h - 7; y++)
    {
        for (double x = 7; x < w - 7; x++)
        {
            if (pixel_grey(image, x, y) < 100)
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
                    couple_list[index_hough].x = x;
                    couple_list[index_hough].y = y;
                    if (biggest_pix < new_pix)
                    {
                        biggest_pix = new_pix;
                    }
                }
            }
        }
    }
    free(thetas);
    double new_theta = 0.;
    for (int i = 0; i < parcours; i++)
    {
        if (couple_list[i].occurence >= biggest_pix*thresh)
        {
            if (new_theta != couple_list[i].theta)
            {
                new_theta = couple_list[i].theta;
            }
            if (couple_list[i].theta < 1.65 && couple_list[i].theta > 1.45)
            {
                for (double x = 1; x < w - 1; x++)
                {
                    double y = (couple_list[i].ro - x*cos(couple_list[i].theta))/sin(couple_list[i].theta);
                    Uint32 pix = SDL_MapRGB(image->format, 255, 0, 0);
                    if (y > 0. && y < h)
                    {
                        put_pixel(image, (int)x, (int)y, pix);
                    }
                }
            }
        }
    }
    free(couple_list);
    free(hough_tr);
    return image;
}

SDL_Surface *hough_alligned_color(SDL_Surface *image)
{
    int w = image->w;
    int h = image->h;
    int max_dist = (int)(round(sqrt(w * w + h * h)));
    int parcours = 2 * max_dist * 181;
    Triple *couple_list = malloc(parcours * sizeof(Triple));
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
    for (double y = 7; y < h - 7; y++)
    {
        for (double x = 7; x < w - 7; x++)
        {
            if (pixel_grey(image, x, y) < 100)
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
                    couple_list[index_hough].x = x;
                    couple_list[index_hough].y = y;
                    if (biggest_pix < new_pix)
                    {
                        biggest_pix = new_pix;
                    }
                }
            }
        }
    }
    free(thetas);
    double new_theta = 0.;
    for (int i = 0; i < parcours; i++)
    {
        if (couple_list[i].occurence >= biggest_pix*0.4)
        {
            if (new_theta != couple_list[i].theta)
            {
                new_theta = couple_list[i].theta;
            }
            if (couple_list[i].theta < 1.7 && couple_list[i].theta > 1.45)
            {
                for (double x = 1; x < w - 1; x++)
                {
                    double y = (couple_list[i].ro - x*cos(couple_list[i].theta))/sin(couple_list[i].theta);
                    Uint32 pix = SDL_MapRGB(image->format, 254, 0, 0);
                    if (y > 0. && y < h)
                    {
                        Uint32 pixel = get_pixel(image,x,y);
                        if(pixel ==  SDL_MapRGB(image->format, 255, 0, 0))
                        {
                            Uint32 pix = SDL_MapRGB(image->format, 0, 255, 0);
                            put_pixel(image, (int)x, (int)y, pix);
                        }
                        else
                        {
                            put_pixel(image, (int)x, (int)y, pix);
                        }
                    }
                }
            }
        }
    }
    free(couple_list);
    free(hough_tr);
    return image;
}

double my_abs1(double value)
{
    if(value < 0.)
    {
     value *= -1.0;
     return value;
    }
    else
    {
     return value;
    }
}

SDL_Surface *hough_transform(SDL_Surface *image,SDL_Surface *cleaned)
{
    int w = image->w;
    int h = image->h;
    //the screen diagonal max_dist
    int max_dist = (int)(round(sqrt(w * w + h * h)));
    printf("max_dist -> %d\n", max_dist);
    size_t parcours = 2 * max_dist * 181;
    Rotated *image_theta = malloc(sizeof(Rotated));
    Triple *couple_list = malloc(parcours * sizeof(Triple));
    double *hough_tr = calloc(parcours,sizeof(double));
    //degree values from -90 to 90 converted to radians
    double *thetas = malloc(sizeof(double) * 181);

    for (double i = 0; i < 181; i++)
    {
        double values = i * M_PI / 180;
        thetas[(int)i] = values;
    }
    //Thetas initialised to have degrees values set for each angle
    int biggest_pix_occurence = 0;
    double most_occurent_theta = 0.;
    for (double y = 7; y < h - 7; y++)
    {
        for (double x = 7; x < w - 7; x++)
        {
            //If the pixel found is black
            if (pixel_grey(image, x, y) < 100)
            {
                //For every possible value of theta (0,180)
                for (size_t theta_in_degrees = 0; theta_in_degrees < 181; theta_in_degrees++)
                {
                    //get the radian value of the theta
                    double current_theta = thetas[theta_in_degrees];
                    //Calculate the ro of this theta at this (x,y) cordinate
                    double ro = x * cos(current_theta) + y * sin(current_theta);
                    //The index of this pixel in a 1D hough space
                    int index_hough = theta_in_degrees * 2 * max_dist + (int)ro + max_dist;
                    //adding 1 to that pixel value at the index_hough possition
                    double new_pix_occurence = ++hough_tr[index_hough];
                    //Save the theta,ro,and occurence into the couple list
                    couple_list[index_hough].occurence = new_pix_occurence;
                    couple_list[index_hough].ro = ro;
                    couple_list[index_hough].theta = current_theta;
                    couple_list[index_hough].x = x;
                    couple_list[index_hough].y = y;
                    //finds the biggest occurence of a certain pixel and it's theta
                    if (biggest_pix_occurence < new_pix_occurence)
                    {
                        biggest_pix_occurence = new_pix_occurence;
                        most_occurent_theta = current_theta;
                    }
                }
            }
        }
    }
    //We no longer need the theta in radians
    free(thetas);
    //int bornes = max_dist/2 ;
    //The size of the new reduced_couple_list
    size_t reduced_couple_list_size = 0.;
    for (size_t i = 0; i < parcours; i++)
    {
        //Put all the values that are larger than biggest_pix_occurence/2 
        if (couple_list[i].occurence >= biggest_pix_occurence*0.4)
        {
            reduced_couple_list_size++;
        }
    }
    //Create the reduced couple list, with the size that has been calculated (couple_number)
    Triple *reduced_couple_list = malloc(sizeof(Triple) * reduced_couple_list_size);
    int theta_numbers = 0;
    double new_theta = 0.;
    //index in the reduced_couple_list
    size_t z = 0;
    for (size_t i = 0; i < parcours; i++)
    {
        //Put all the values that are larger than biggest_pix_occurence/2 
        if (couple_list[i].occurence >= biggest_pix_occurence*0.4)
        {
            reduced_couple_list[z] = couple_list[i];
        
            //Count the number of occurences of different theta values
            if (new_theta != reduced_couple_list[z].theta)
            {
                new_theta = reduced_couple_list[z].theta;
                theta_numbers += 1.;
            }
            z++;
        }
    }

    Triple *grouped = malloc(sizeof(Triple) * (theta_numbers + 1));
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
    //printf("z -> %zu", z);
    //Scan through the list
    for (size_t i = 0; i < reduced_couple_list_size; i++)
    {
        //If we see a theta that has not been seen before
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
        //Else it is the same theta
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
        if(grouped[i].theta!=0.){
        }
    }
    if ((grouped[biggest_theta_occurence].theta < 1.65 && grouped[biggest_theta_occurence].theta > 1.45) || grouped[biggest_theta_occurence].theta == 0)
    {
       printf("The sudoku is alligned\n");
        image = hough_alligned(image,0.4);
        image = rotation_90(image);
        image = hough_alligned_color(image);
        image = rotation_270(image);
        SDL_SaveBMP(image,"images/coloured.bmp");
        SDL_SaveBMP(cleaned,"images/rotated.bmp");
        image = trim_green(image,cleaned);
        image_theta->image_output = image;
        image_theta->theta = -1;
    }
    else
    {
	    image = rotate(image, grouped[biggest_theta_occurence].theta);
        SDL_SaveBMP(image,"images/rotated_sobel.bmp");
        image = hough_alligned(image,0.4);
        image = rotation_90(image);
        image = hough_alligned_color(image);
        image = rotation_270(image);
        SDL_SaveBMP(image,"images/coloured.bmp");
        cleaned = rotate(cleaned,grouped[biggest_theta_occurence].theta);
        SDL_SaveBMP(cleaned,"images/rotated.bmp");
        image = trim_green(image,cleaned);
        SDL_SaveBMP(image,"images/trimmed.bmp");
        image_theta->image_output = image;
        image_theta->theta = -1;
    }
    free(grouped);
    free(reduced_couple_list);
    free(couple_list);
    free(hough_tr);
    //free(strait_angle);
    return image_theta->image_output;
}


 