
#define _GNU_SOURCE
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "basic_pixel.h"
#include <stdio.h>
#include <err.h>

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
char *create_str1(int number, char *str_path){
	char *output = NULL;
	asprintf(&output,"%s%d.bmp",str_path,number);
	if(!output){
		errx(1,"Output is null");
	}
	return output;

}
char *create_str(int number, char *str_path)
{
    //Initialise a pointer/iterable for str_path
    size_t str_path_len = 0;
    //calculate it's length without the \0
    while (str_path[str_path_len] != 0){
        str_path_len++;
    }
    // The format of the file to save 
    printf("str_path_len -> %zu\n",str_path_len);
    char str_bmp[] = ".bmp";
    //Initialise a pointer/iterable for str_bmp
    size_t str_bmp_len = 0;
    //calculate it's length without the \0
    while (str_bmp[str_bmp_len] != 0){
        str_bmp_len++;
    }
    printf("str_bmp_len -> %zu\n",str_bmp_len);
    //Remeber the number, to not loose it after we count its digits
    int rem_num = number;
    // count is a counter of digits
    size_t count = 0;
    //THe loop that counts digits and saves in count
    while (number != 0)
    {
        count++;
        number /= 10;
    }
    //We can now malloc the memory for the number + the \0
    char *str = malloc((count + 1) * sizeof(char));
    str[count] = '\0';
    //Convert the int number to char[count] number 
    // and save it in the string
    for (size_t i = count; i > 0; i--)
    {
        str[i - 1] = (rem_num - (rem_num / 10) * 10) + '0';
        rem_num /= 10;
    }
    printf("count -> %zu\n",count);
    //Initialise the final size of the output string
    size_t str_path_malloc_len = str_path_len +1+ count + str_bmp_len;
    //Malloc it into the memory
    char *str_path_malloc = malloc((str_path_malloc_len) * sizeof(char));
    str_path_malloc[str_path_malloc_len-1] = '\0';
    //We start by adding ".bmp" at the end of the string
    for (size_t i = str_bmp_len; i > 0; i--){
        str_path_malloc[--str_path_malloc_len] = str_bmp[i - 1];
    }
    //tr_path_malloc[str_path_malloc_len]
    //We than add the number
    for (size_t i = count; i > 0; i--){ 
        str_path_malloc[--str_path_malloc_len] = str[i - 1];
    }
    // We have added the number to the final string,
    //So we can now free it
    free(str);
    //We add the path to the string
    for (size_t i = str_path_len; i > 0; i--){

        str_path_malloc[--str_path_malloc_len] = str_path[i - 1];
    }
    return str_path_malloc;
}
void detect_green(SDL_Surface *image_green, SDL_Surface *original,char *str_path)
{
    printf("%s , str\n",str_path);
    size_t h = image_green->h;
    size_t w = image_green->w;
    // Count_numbers represents the case number
    int count_numbers = 0;
    size_t new_i = 0;
    size_t j = 0;
    while (j < h)
    {
        size_t i = new_i;
        while (i < w)
        {
            //Get the pixel at i,j
            Uint32 pixel = get_pixel(image_green, i, j);
            //If it is not a detected pixel, increment j
            //else if it is a detected pixel
            if (pixel == SDL_MapRGB(image_green->format, 100, 255, 100))
            {
                //Find the hight from the green we detected
                size_t new_detected_h = detect_green_vert(image_green, i, j);
                //Find the width from the green we detected
                size_t new_detected_w = detect_green_hor(image_green, i, j);
                //verify that this could be a cube
                //If it can't be one increment j
                //To look at the next pixel and go
                //To the next loop

                double divh_w = (double)new_detected_h / (double)new_detected_w;
                double divw_h = (double)new_detected_w / (double)new_detected_h;

                if ((divh_w < 0.85 || divw_h < 0.85) || ((double)new_detected_h < (h/33.) || (double)new_detected_w < (w/33.)))
                {

                    i += 1;
                    continue;
                }
                printf(" number %d -> corner i %zu, j %zu\n", count_numbers, i, j);
                printf("h/w -> %f , w/h -> %f, new_h -> %zu , new_w -> %zu\n", divh_w, divw_h, new_detected_h, new_detected_w);
                printf("%d\n", ((double)new_detected_h < 40. || (double)new_detected_w < 40.));

                //Else it looks like a cube structure
                //Create a placement for it
                SDL_Surface *number = SDL_CreateRGBSurface(0, new_detected_w, new_detected_h, 32, 0, 0, 0, 0);
                //Set an index indicating the end of the sequence vertically
                size_t index_j = new_detected_h + j;
                //Set an index indicating the end of the sequence horizontally
                size_t index_i = new_detected_w + i;
                // Memorise both current i and j
                size_t remeber_i = i;
                size_t remebre_j = j;
                //Initialise the index counters for the sequence at 0
                size_t num_i = 0;
                size_t num_j = 0;
                //Start looping throught the sequence
                while (j < index_j)
                {
                    //Start looping through the sequence
                    while (i < index_i)
                    {
                        //Get the pixel from the original image
                        pixel = get_pixel(original, i, j);
                        // Put the pixel at it's placement in the
                        //number SDL_Surface
                        put_pixel(number, num_i, num_j, pixel);
                        //At the same time mark with green the place
                        // From which you have extracted the pixel
                        //That was put into number
                        Uint32 pixel1 = SDL_MapRGB(image_green->format, 0, 0, 255);
                        put_pixel(original, i, j, pixel1);
                        put_pixel(image_green, i, j, pixel1);
                        // increment both i and num_i indexes
                        i += 1;
                        num_i += 1;
                    }
                    //A line has been ran through
                    //So num_i is put to zero
                    num_i = 0;
                    //And i is restored
                    i = remeber_i;
                    // Increment j
                    j += 1;
                    num_j += 1;
                }
               char *str = create_str1(count_numbers,str_path);
                SDL_SaveBMP(number, str);
                free(str);
                SDL_FreeSurface(number);

                count_numbers += 1;

                if (count_numbers + 1 % 9 == 0)
                {
                    i = 0;
                    j = index_j;
                }
                if (count_numbers + 1 % 9 != 0)
                {
                    i = index_i;
                    j = remebre_j;
                }
                pixel = get_pixel(image_green, i, j);
            }
            i += 1;
        }
        j += 1;
    }
}

//This function will clean up the image after detect_green
//Getting rid of purple pixels adjacent to greens,
//As well as this strips of white and red pixels
SDL_Surface *clean_up(SDL_Surface *image)
{
    size_t h = image->h;
    size_t w = image->w;
    Uint32 green_pix = SDL_MapRGB(image->format, 100, 255, 100);
    for (size_t i = 1; i < w - 1; i++)
    {
        for (size_t j = 1; j < h - 1; j++)
        {
            Uint32 pix_to_test = get_pixel(image, i, j);
            if (pix_to_test == SDL_MapRGB(image->format, 100, 0, 100) ||
                // pix_to_test == SDL_MapRGB(image->format, 255, 255, 255) ||
                pix_to_test == SDL_MapRGB(image->format, 0, 255, 0) ||
                pix_to_test == SDL_MapRGB(image->format, 0, 0, 0))
            {
                if (get_pixel(image, i, j - 1) == green_pix
                || get_pixel(image, i, j + 1) == green_pix 
                || get_pixel(image, i - 1, j) == green_pix 
                || get_pixel(image, i + 1, j) == green_pix )
                {
                    put_pixel(image, i, j, green_pix);
                }
            }
            if (pix_to_test == SDL_MapRGB(image->format, 255, 255, 255))
            {
                if (
                    get_pixel(image, i + 1, j) == green_pix && get_pixel(image, i - 1, j) == green_pix)
                {
                    put_pixel(image, i, j, green_pix);
                }
            }
            
        }
    }
    return image;
}
