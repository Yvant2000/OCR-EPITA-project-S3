#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "basic_pixel.h"
#include "rotation.h"

void print_m_c(char *name, double m_c[2])
{
    printf("%s -> m = %f , c = %f\n", name, m_c[0], m_c[1]);
}

void trace_line(SDL_Surface *image, double m_c[2], double x2, double y2, double x1, double y1)
{
    double h = image->h;
    double bot = x2 - x1;
    double m = 0.;
    if (bot != 0)
    {
        m = (y2 - y1) / bot;
    }
    double c = y1 - m * x1;
    m_c[0] = m;
    m_c[1] = c;
    //print_m_c("top",m_c);
    double first_x;
    double last_x;
    if (x2 > x1)
    {
        first_x = x1;
        last_x = x2;
    }
    else
    {
        last_x = x1;
        first_x = x2;
    }
    for (double x = first_x; x < last_x; x += 0.0005)
    {
        double y1 = m_c[0] * x + m_c[1];
        if (y1 < h && y1 > 0.)
        {
            Uint32 pixel = SDL_MapRGB(image->format, 255, 0, 0);
            //printf("x = %f , y = %f\n",x,y1);
            put_pixel(image, (int)x, (int)y1, pixel);
        }
    }
}

SDL_Surface *clean_boundaries(SDL_Surface *image)
{
    size_t h = image->h;
    size_t w = image->w;
    for (size_t j = h - 1; j > 0; j--)
    {
        for (size_t i = 0; i < w; i++)
        {
            Uint32 pix = get_pixel(image, i, j);
            //If the pixel is not black
            if (pix != SDL_MapRGB(image->format, 255, 0, 0))
            {
                //trim here on the bottom
                Uint32 pixel = SDL_MapRGB(image->format, 255, 255, 255);
                put_pixel(image, i, j, pixel);
            }
            else
            {
                break;
            }
        }
    }
    for (size_t j = 0; j < h; j++)
    {
        for (size_t i = 0; i < w; i++)
        {
            Uint32 pix = get_pixel(image, i, j);
            //If the pixel is not black
            //If the pixel is not black
            if (pix != SDL_MapRGB(image->format, 255, 0, 0))
            {
                //trim here on the bottom
                Uint32 pixel = SDL_MapRGB(image->format, 255, 255, 255);
                put_pixel(image, i, j, pixel);
            }
            else
            {
                break;
            }
        }
    }
    for (size_t j = 0; j < h; j++)
    {
        for (size_t i = w - 1; i > 0; i--)
        {
            Uint32 pix = get_pixel(image, i, j);
            //If the pixel is not black
            //If the pixel is not black
            if (pix != SDL_MapRGB(image->format, 255, 0, 0))
            {
                //trim here on the bottom
                Uint32 pixel = SDL_MapRGB(image->format, 255, 255, 255);
                put_pixel(image, i, j, pixel);
            }
            else
            {
                break;
            }
        }
    }
    for (size_t j = h - 1; j > 0; j--)
    {
        for (size_t i = w - 1; i > 0; i--)
        {
            Uint32 pix = get_pixel(image, i, j);
            //If the pixel is not black
            if (pix != SDL_MapRGB(image->format, 255, 0, 0))
            {
                //trim here on the bottom
                Uint32 pixel = SDL_MapRGB(image->format, 255, 255, 255);
                put_pixel(image, i, j, pixel);
            }
            else
            {
                break;
            }
        }
    }

    for (size_t j = 0; j < h; j++)
    {
        for (size_t i = 0; i < w; i++)
        {
            Uint32 pix = get_pixel(image, i, j);
            //If the pixel is not black
            //If the pixel is not black
            if (pix == SDL_MapRGB(image->format, 255, 0, 0))
            {
                //trim here on the bottom
                Uint32 pixel = SDL_MapRGB(image->format, 255, 255, 255);
                put_pixel(image, i, j, pixel);
            }
        }
    }
    return image;
}

double *find_top_left_corner(SDL_Surface *image)
{
    double h = image->h;
    double *borders = malloc(2 * sizeof(double));
    double dec = 1;
    while (dec < h)
    {
        double nk = 0;
        double rem = dec;
        while (dec > 0)
        {
            Uint32 pix = get_pixel(image, nk, dec);
            //If the pixel is green
            if (pix == SDL_MapRGB(image->format, 0, 255, 0))
            {
                borders[0] += nk;
                borders[1] += dec;
                //print_m_c("find_top_left_corner",borders);
                return borders;
            }
            dec--;
            nk++;
        }
        dec = rem + 1;
    }
    return borders;
}

double *find_bot_right_corner(SDL_Surface *image)
{
    double h = image->h;
    double w = image->w;
    double *borders = malloc(2 * sizeof(double));
    double dec = h - 1;
    while (dec > 0)
    {
        double nk = w - 1;
        double rem = dec;
        while (dec < h)
        {
            Uint32 pix = get_pixel(image, nk, dec);
            //If the pixel is green
            if (pix == SDL_MapRGB(image->format, 0, 255, 0))
            {
                borders[0] += nk;
                borders[1] += dec;
                //print_m_c("find_bot_right_corner",borders);
                return borders;
            }
            dec++;
            nk--;
        }
        dec = rem - 1;
    }
    return borders;
}

double *find_bot_left_corner(SDL_Surface *image)
{
    double h = image->h;
    double w = (image->w) - 1;
    double *borders = malloc(2 * sizeof(double));
    double dec = 1;
    while (dec < w)
    {
        double nk = h - 1;
        double rem = dec;
        while (dec > 0)
        {
            Uint32 pix = get_pixel(image, dec, nk);
            //If the pixel is green
            if (pix == SDL_MapRGB(image->format, 0, 255, 0))
            {
                borders[0] += dec;
                borders[1] += nk;
                //print_m_c("find_bot_left_corner",borders);
                return borders;
            }
            dec--;
            nk--;
        }
        dec = rem + 1;
    }
    return borders;
}

double *find_top_right_corner(SDL_Surface *image)
{
    double h = image->h;
    double w = (image->w) - 1;
    double *borders = malloc(2 * sizeof(double));
    double dec = 1;
    while (dec < h)
    {
        double nk = w;
        double rem = dec;
        while (dec > 0)
        {
            Uint32 pix = get_pixel(image, nk, dec);
            //If the pixel is green
            if (pix == SDL_MapRGB(image->format, 0, 255, 0))
            {
                borders[0] += nk;
                borders[1] += dec;
                //print_m_c("find_top_right_corner",borders);
                return borders;
            }
            dec--;
            nk--;
        }
        dec = rem + 1;
    }
    return borders;
}

void set_boundaries(SDL_Surface *image, double boundaries[8])
{
    double *border_bot_right = find_bot_right_corner(image);
    double *border_top_right = find_top_right_corner(image);
    double *border_bot_left = find_bot_left_corner(image);
    double *border_top_left = find_top_left_corner(image);
    boundaries[0] = border_top_left[0];
    boundaries[1] = border_top_left[1];
    boundaries[2] = border_top_right[0];
    boundaries[3] = border_top_right[1];
    boundaries[4] = border_bot_left[0];
    boundaries[5] = border_bot_left[1];
    boundaries[6] = border_bot_right[0];
    boundaries[7] = border_bot_right[1];
    free(border_bot_left);
    free(border_bot_right);
    free(border_top_left);
    free(border_top_right);
}

void change_base(double boundaries[8], int h)
{
    double top_l_y = boundaries[1];
    double top_l_x = boundaries[0];
    boundaries[1] = top_l_x;
    boundaries[0] = h - top_l_y - 1;

    double top_r_y = boundaries[3];
    double top_r_x = boundaries[2];
    boundaries[2] = top_r_x;
    boundaries[3] = h - top_r_y - 1;

    double bot_l_y = boundaries[5];
    double bot_l_x = boundaries[4];

    boundaries[4] = bot_l_x;
    boundaries[5] = h - bot_l_y - 1;

    double bot_r_y = boundaries[7];
    double bot_r_x = boundaries[6];
    boundaries[6] = bot_r_x;
    boundaries[7] = h - bot_r_y - 1;
}

SDL_Surface *is_in_boundaries(SDL_Surface *image, SDL_Surface *colored, double boundaries[8])
{
    int h = image->h;
    set_boundaries(colored, boundaries);

    //Find the 4 function equations
    double top_l_x = boundaries[0];
    double top_l_y = boundaries[1];
    //printf("top_l x -> %f , y -> %f \n",top_l_x,top_l_y);

    double top_r_x = boundaries[2];
    double top_r_y = boundaries[3];
    //printf("top_r x -> %f , y -> %f \n",top_r_x,top_r_y);

    double bot_l_x = boundaries[4];
    double bot_l_y = boundaries[5];
    //printf("bot_l x -> %f , y -> %f \n",bot_l_x,bot_l_y);

    double bot_r_x = boundaries[6];
    double bot_r_y = boundaries[7];

    //printf("bot_r x -> %f , y -> %f \n",bot_r_x,bot_r_y);

    //left line
    double left_m_c[2] = {0., 0.};
    //print_m_c("left",left_m_c);
    //right line
    double right_m_c[2] = {0., 0.};
    //print_m_c("right",right_m_c);
    //top line
    double top_m_c[2] = {0., 0.};
    //print_m_c("top",top_m_c);
    //bot line
    double bot_m_c[2] = {0., 0.};
    //print_m_c("bot",bot_m_c);

    //trace_line(image,left_m_c,top_l_x,top_l_y,bot_l_x,bot_l_y);
    //trace_line(image,right_m_c,top_r_x,top_r_y,bot_r_x,bot_r_y);
    trace_line(image, top_m_c, top_r_x, top_r_y, top_l_x, top_l_y);
    trace_line(image, bot_m_c, bot_r_x, bot_r_y, bot_l_x, bot_l_y);
    image = rotation_90(image);
    change_base(boundaries, h);
    top_l_x = boundaries[1];
    top_l_y = boundaries[0];
    //printf("top_l x -> %f , y -> %f \n",top_l_x,top_l_y);

    top_r_x = boundaries[2];
    top_r_y = boundaries[3];
    //printf("top_r x -> %f , y -> %f \n",top_r_x,top_r_y);

    bot_l_x = boundaries[4];
    bot_l_y = boundaries[5];
    //printf("bot_l x -> %f , y -> %f \n",bot_l_x,bot_l_y);

    bot_r_x = boundaries[6];
    bot_r_y = boundaries[7];

    //printf("bot_r x -> %f , y -> %f \n",bot_r_x,bot_r_y);
    trace_line(image, left_m_c, bot_l_y, bot_l_x, top_l_y, top_l_x);
    trace_line(image, right_m_c, bot_r_y, bot_r_x, top_r_y, top_r_x);
    image = rotation_270(image);

    image = clean_boundaries(image);
    return image;
}

SDL_Surface *crop_the_image(SDL_Surface *image, SDL_Surface *colored)
{
    double bound[8] = {0., 0., 0., 0., 0., 0., 0., 0.};
    image = is_in_boundaries(image, colored, bound);
    SDL_FreeSurface(colored);
    return image;
}
