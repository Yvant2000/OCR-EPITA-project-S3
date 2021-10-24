#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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
SDL_Surface *trim(SDL_Surface *image)
{
    size_t h = image->h;
    size_t w = image->w;
    size_t bottom_trim = 0;
    size_t top_trim = 0;
    size_t right_trim = 0;
    size_t left_trim = 0;

    for (size_t j = h-1; j > 0; j--)
    {
        for (size_t i = 0; i < w; i++)
        { //printf("bottom i -> %zu , j -> %zu\n",i,j);
            Uint32 pix = get_pixel(image, i, j);
            //If the pixel is not black
            if (pix != SDL_MapRGB(image->format, 0, 0, 0))
            {
                //trim here on the bottom
                bottom_trim = j - 1;
            }
        }
    }
    for (size_t j = 0; j < h; j++)
    {
        for (size_t i = 0; i < w; i++)
        { //printf("top i -> %zu , j -> %zu\n",i,j);
            Uint32 pix = get_pixel(image, i, j);
            //If the pixel is not black
            if (pix != SDL_MapRGB(image->format, 0, 0, 0))
            {
                //trim here on the top
                top_trim = j - 1;
            }
        }
    }
    for (size_t i = 0; i < w; i++)
    {
        for (size_t j = top_trim; j > bottom_trim; j--)
        { //printf("right i -> %zu , j -> %zu\n",i,j);
            Uint32 pix = get_pixel(image, i, j);
            //If the pixel is not black
            if (pix != SDL_MapRGB(image->format, 0, 0, 0))
            {
                //trim here on the right
                right_trim =  i - 1;
            }
        }
    }
    for (size_t i = w-1; i > 0; i--)
    {
        for (size_t j = top_trim; j > bottom_trim; j--)
        { //printf("left i -> %zu , j -> %zu\n",i,j);
            Uint32 pix = get_pixel(image, i, j);
            //If the pixel is not black
            if (pix != SDL_MapRGB(image->format, 0, 0, 0))
            {
                //trim here on the left
                left_trim =  i - 1;
            }
        }
    }
    SDL_Surface *output_image = SDL_CreateRGBSurface(0, w-(right_trim-left_trim), h - (top_trim-bottom_trim), 32, 0, 0, 0, 0);
    for (size_t i = left_trim; i < right_trim; i++)
    {
        for (size_t j = bottom_trim; j < top_trim; j++)
        {
            Uint32 pix = get_pixel(image, i, j);
            //If the pixel is not black
            put_pixel(output_image,i-left_trim,j-bottom_trim,pix);
        }
    }
    return output_image;
}


int *new_sheer(double angle, int x, int y)
{
    int *x_and_y = malloc(sizeof(int) * 2);
    double intan = angle / 2.f;
    double tangent = tan(intan);
    int new_x = round((double)x - ((double)y * tangent));
    int new_y = y;
    new_y = round((double)new_x * sin(angle) + (double)new_y);
    new_x = round((double)new_x - ((double)new_y * tangent));
    x_and_y[0] = new_x;
    x_and_y[1] = new_y;
    return x_and_y;
}
double my_abs(double value)
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



SDL_Surface *rotation_90(SDL_Surface *image)
{
    int w = image->w;
    int h = image->h;
    SDL_Surface *rotated = SDL_CreateRGBSurface(0, h, w, 32, 0, 0, 0, 0);
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            Uint32 pix = get_pixel(image, i, j);
            put_pixel(rotated, h - j - 1, i, pix);
        }
    }
    return rotated;
}

SDL_Surface *rotation_180(SDL_Surface *image)
{
    image = rotation_90(image);
    image = rotation_90(image);
    return image;
}

SDL_Surface *rotation_270(SDL_Surface *image)
{
    int w = image->w;
    int h = image->h;
    SDL_Surface *rotated = SDL_CreateRGBSurface(0, h, w, 32, 0, 0, 0, 0);
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            Uint32 pix = get_pixel(image, i, j);
            put_pixel(rotated, j, w - 1 - i, pix);
        }
    }
    return rotated;
}

SDL_Surface *rotate1(SDL_Surface *image, double angle)
{
	angle = 360. - angle;
    if(angle == 270.)
     {
        return rotation_270(image);
     }
     if(angle == 180.)
     {
         return rotation_180(image);
     }
     if(angle == 90.)
     {
         return rotation_90(image);
     }
    
    double cosine = cos(angle);
    double sinus = sin(angle);
    int height = image->h;
    int width = image->w;
    int new_height = round(my_abs(height * cosine) +
            my_abs(width * sinus)) + 1;
    int new_width = round(my_abs(width * cosine) +
            my_abs(height * sinus)) + 1;
    SDL_Surface *output_image = SDL_CreateRGBSurface(0,
           2* new_width , 2*new_height , 32, 0, 0, 0, 0);
    int original_center_height = round(((height + 1) / 2) - 1);

    int original_center_width = round(((width + 1) / 2) - 1);
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
        int *x_and_y = malloc(sizeof(int) * 2);
            int y = height - 1 - j - original_center_height;
            int x = width - 1 - i - original_center_width;
            x_and_y = new_sheer(angle, x, y);
            int new_x = x_and_y[0];
            int new_y = x_and_y[1];
            new_y = new_height - new_y;
            new_x = new_width - new_x;
         free(x_and_y);
            Uint32 pixel = get_pixel(image, i, j);
            put_pixel(output_image, new_x, new_y, pixel);
        }
    }
    SDL_Surface *test = trim(output_image);
    return test;
}

SDL_Surface *rotate(SDL_Surface *image, double angle)
{
    if(angle == 270.)
    {
        return rotation_270(image);
    }
    if(angle == 180.)
    {
        return rotation_180(image);
    }
    if(angle == 90.)
    {
        return rotation_90(image);
    }
    int *x_and_y = malloc(sizeof(int) * 2);
    angle = (angle * 3.14159265)/180;
    double cosine = cos(angle);
    double sinus = sin(angle);
    double height = image->h;
    double width = image->w;
    int new_height = round(my_abs(height * cosine) +
			    my_abs(width * sinus)) + 1;
    int new_width = round(my_abs(width * cosine) +
			    my_abs(height * sinus)) + 1;
    SDL_Surface *output_image = SDL_CreateRGBSurface(
		    0, new_width * 2, new_height * 2, 32, 0, 0, 0, 0);
    int original_center_height = round(((height + 1) / 2) - 1);
    
    int original_center_width = round(((width + 1) / 2) - 1);
    
   for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int y = height - 1 - i - original_center_height;
            int x = width - 1 - j - original_center_width;
            x_and_y = new_sheer(angle, x, y);
            int new_x = x_and_y[0];
            int new_y = x_and_y[1];
            new_y = new_height - new_y;
            new_x = new_width - new_x;
            Uint32 pixel = get_pixel(image, i, j);
            put_pixel(output_image, new_y, new_x, pixel);
        }
    }
    free(x_and_y);
     SDL_Surface *test = trim(output_image);
    return test;
}


int str_to_int(char *num)
{
    size_t i = 0;
    int number = 0;
    while (num[i] != 0)
    {
        number *= 10;
        int digit = num[i] - '0';
        number += digit;
        i+=1;
    }
    return number;
}
