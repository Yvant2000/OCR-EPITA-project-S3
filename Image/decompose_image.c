#define _GNU_SOURCE
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <math.h>
#include <err.h>
#include <limits.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include "kernel.h"
#include "primary_filters.h"
#include "otsu_binary.h"
#include "rotation.h"
#include "sobel.h"
#include "hough_functions.h"
#include "corners.h"
#include "image_maker.h"

typedef struct Sobel
{
    SDL_Surface *gradient_intensity;
    double *directions;
} Sobel;

typedef struct Triple
{
    double theta;
    double ro;
    int occurence;
} Triple;

typedef struct Couple
{
    int x;
    int y;
} Couple;

typedef struct Matrix_couple
{
    Couple left_top;
    Couple left_bot;
    Couple right_top;
    Couple right_bot;

} Matrix_couple;

struct list
{
    struct list *next;
    size_t debut;
    size_t fin;
    int pos;
};

struct list *empty_list()
{
    return NULL;
}

struct list *init_list(size_t debut, size_t fin, int pos)
{
    struct list *l;
    l = malloc(sizeof(struct list));
    l->debut = debut;
    l->fin = fin;
    l->pos = pos;
    l->next = NULL;
    return l;
}

int list_is_empty(struct list *l)
{
    return l == NULL;
}

struct list *add(struct list *l, size_t x, size_t y, int pos)
{
    struct list *tmp;
    tmp = malloc(sizeof(struct list));
    tmp->debut = x;
    tmp->fin = y;
    tmp->pos = pos;
    tmp->next = l;
    return tmp;
}

size_t list_len(struct list *l)
{
    size_t len;
    for (len = 0; l; l = l->next)
        len += 1;
    return len;
}
void free_list(struct list *head)
{
    struct list *curr;
    while ((curr = head) != NULL)
    {                      // set curr to head, stop if list empty.
        head = head->next; // advance head to next element.
        free(curr);        // delete saved pointer.
    }
}
size_t *list_sum(struct list *l)
{
    size_t *sum = malloc(sizeof(size_t) * 2);
    size_t x = 0;
    size_t y = 0;
    for (; l; l = l->next)
    {
        x += l->debut;
        y += l->fin;
    }
    sum[0] = x;
    sum[1] = y;
    return sum;
}

Matrix_couple *init_list_matrix(struct list *final)
{
    size_t len_list = list_len(final);
    Couple *couple = malloc(len_list * sizeof(Couple));
    for (; final; final = final->next)
    {
        Couple coup;
        coup.x = (int) final->debut;
        coup.y = (int) final->fin;
        //printf("pos -> %d x -> %d y -> %d\n", final->pos, coup.x, coup.y);
        couple[final->pos] = coup;
    }
    Matrix_couple *matrixes = calloc(90, sizeof(Matrix_couple));
    int count = 1;
    while ((count + 11) <= (int)len_list && count < 90)
    {
        if (count % 10 == 0)
        {
            Matrix_couple matrix;
            matrix.left_top = couple[0];
            matrix.left_bot = couple[0];
            matrix.right_top = couple[0];
            matrix.right_bot = couple[0];
            matrixes[count] = matrix;
            count++;
        }
        Matrix_couple matrix;
        matrix.left_top = couple[count];
        matrix.left_bot = couple[count + 10];
        matrix.right_top = couple[count + 1];
        matrix.right_bot = couple[count + 11];
        matrixes[count - 1] = matrix;
        count++;
    }
    free_list(final);
    free(couple);
    return matrixes;
}

Matrix_couple *init_list_matrix_red(struct list *final)
{
    size_t len_list = list_len(final);
    Couple *couple = malloc(len_list * sizeof(Couple));
    for (; final; final = final->next)
    {
        Couple coup;
        coup.x = (int) final->debut;
        coup.y = (int) final->fin;
        //printf(" init list matrix pos -> %d x -> %d y -> %d\n", final->pos, coup.x, coup.y);
        couple[final->pos] = coup;
    }
    /*
    for (size_t i = 0; i < len_list; i++)
    {
        printf(" couple: %ld x -> %d , y -> %d\n", i, couple[i].x, couple[i].y);
    }
    */
    Matrix_couple *matrixes = calloc(19, sizeof(Matrix_couple));
    int count = 1;
    while ((count + 5) <= 16)
    {
        if (count % 4 == 0)
        {
            Matrix_couple matrix;
            matrix.left_top = couple[0];
            matrix.left_bot = couple[0];
            matrix.right_top = couple[0];
            matrix.right_bot = couple[0];
            matrixes[count] = matrix;
            count++;
        }
        Matrix_couple matrix;
        matrix.left_top = couple[count];
        matrix.left_bot = couple[count + 4];
        matrix.right_top = couple[count + 1];
        matrix.right_bot = couple[count + 5];
        matrixes[count - 1] = matrix;
        count++;
    }
    free_list(final);
    free(couple);
    return matrixes;
}

char *create_str(int number, char *str_path)
{
    char *output = NULL;
    if (number < 10)
    {
        asprintf(&output, "%s0%d.bmp", str_path, number);
        if (!output)
        {
            errx(1, "Output is null");
        }
    }
    else
    {
        asprintf(&output, "%s%d.bmp", str_path, number);
        if (!output)
        {
            errx(1, "Output is null");
        }
    }
    //printf("%s\n",output);
    return output;
}

void save_in_dir(char dir[PATH_MAX],char *name,char output[PATH_MAX])
{
    strcpy(output,dir);
    strcat(output,name);
}

void prepare_string(char *str_path,char save[PATH_MAX])
{
    char real_path[PATH_MAX];
    realpath(str_path,real_path);
    //printf("%s\n",real_path);
    char *dir_name = dirname(real_path);
    //printf("%s\n",dir_name);
    strcpy(save,dir_name);
    strcat(save,"/numbers/num");
    //printf("%s\n",save);
}

void directory(char *str_path,char save[PATH_MAX])
{
    char real_path[PATH_MAX];
    realpath(str_path,real_path);
    char *dir_name = dirname(real_path);
    strcpy(save,dir_name);
}

SDL_Surface *rescale(SDL_Surface *picture, int newwidth, int newheight)
{
    Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN //post-parsing
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    SDL_Surface *newpic = SDL_CreateRGBSurface(0, newwidth, newheight, 32, rmask, gmask, bmask, amask);
    for (int i = 0; i < newwidth; i++)
    {
        for (int j = 0; j < newheight; j++)
        {
            int x = (int)((double)i / (double)newwidth * (double)picture->w);
            int y = (int)((double)j / (double)newheight * (double)picture->h);
            set_pixel(newpic, i, j, get_pixel(picture, x, y));
        }
    }
    return newpic;
}

int *semi_elementary_cordinates()
{
    int *last = malloc(81 * sizeof(int));
    int last_val = 18;
    int val = 1;
    int ct = 0;
    while (val < 81)
    {
        for (size_t k = 0; k < 3; k++)
        {
            val = last_val - 17;
            for (int i = 0; i < 3; i++)
            {
                for (size_t j = 0; j < 3; j++)
                {
                    last[ct] = val;
                    ct++;
                    val++;
                }
                val += 6;
            }
            last_val = val;
            last_val -= 7;
        }
        last_val += 18;
    }
    return last;
}

void segmenting(SDL_Surface *image,char *absolute_path)
{
    char abs_path[PATH_MAX];
    prepare_string(absolute_path,abs_path);
    printf("%s\n",abs_path);
    int w = image->w;
    int h = image->h;
    int pos = 0;
    int del = 0;
    
    struct list *beg_lines = init_list(0, 0, 0);
    struct list *save_beg = beg_lines;
    struct list *borned = init_list(0, 0, 0);
    struct list *true_list = init_list(0, 0, 0);
    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w / 9; i++)
        {
            Uint32 pix = get_pixel(image, i, j);
            if (pix == SDL_MapRGB(image->format, 0, 255, 0))
            {
                beg_lines = add(beg_lines, i, j, pos);
                borned = add(borned, i, j, pos);
                pos++;
            }
        }
    }
    struct list *save = borned;
    int st = borned->fin;
    borned = borned->next;
    int ct = 1;
    for (; borned; borned = borned->next)
    {
        if (borned->debut == 0 && borned->fin == 0)
        {
            continue;
        }
        del += (st - borned->fin);
        st = borned->fin;
        ct += 1;
    }
    free_list(save);
    //printf("ct->%d\n", ct);
    if (ct == 5)
    {
        del /= 6;
        del *= 0.2;
        //printf("del -> %d\n", del);
        pos = 13;
        int incr = 8;
        for (; beg_lines; beg_lines = beg_lines->next)
        {
            if ((beg_lines->fin == 0 && beg_lines->debut == 0) || beg_lines->pos == 0)
            {
                continue;
            }
            int born_inf = (beg_lines->fin) - del;
            int born_sup = (beg_lines->fin) + del;
            if (born_inf < 0)
            {
                born_inf = 0;
            }
            if (born_sup > h)
            {
                born_sup = h;
            }
            //printf("inf -> %d, sup -> %d\n", born_inf, born_sup);
            //printf(" after born_sup pos -> %d , x-> %d , y -> %d \n", beg_lines->pos, beg_lines->debut, beg_lines->fin);
            for (int i = 0; i < w; i++)
            {
                for (int j = born_inf; j < born_sup; j++)
                {
                    Uint32 pix = get_pixel(image, i, j);
                    if (pix == SDL_MapRGB(image->format, 0, 255, 0))
                    {
                        true_list = add(true_list, i, j, pos);
                        //printf("individual pos -> %d, i -> %d , j -> %d\n", pos, i, j);
                        pos++;
                    }
                }
            }
            pos = pos - incr;
        }
        free_list(save_beg);
        Matrix_couple *coord = init_list_matrix_red(true_list);
        free_list(true_list);
        int numb = 0;
        int *vals = semi_elementary_cordinates();
        for (size_t i = 0; i < 19; i++)
        {
            int bot_dif = coord[i].right_bot.x - coord[i].left_bot.x;
            int top_dif = coord[i].right_top.x - coord[i].left_top.x;
            int left_dif = coord[i].left_bot.y - coord[i].left_top.y;
            int right_dif = coord[i].right_bot.y - coord[i].right_top.y;
            if (bot_dif == 0 || top_dif == 0 || left_dif == 0 || right_dif == 0)
            {
                continue;
            }
            /*
            else
            {
                printf("case: %d left_top -> (%d,%d) ,left_bot -> (%d,%d) ,right_top -> (%d,%d) ,right_bot -> (%d,%d) , \n",
                       numb,
                       coord[i].left_top.x, coord[i].left_top.y, coord[i].left_bot.x, coord[i].left_bot.y,
                       coord[i].right_top.x, coord[i].right_top.y, coord[i].right_bot.x, coord[i].right_bot.y);
            }
            */
            //printf("difs: bot -> %d ,  top-> %d , left -> %d , right -> %d\n", bot_dif, top_dif, left_dif, right_dif);
            int dec_w = coord[i].left_top.x;
            int dec_h = coord[i].right_top.y;
            int final_h = right_dif;
            int final_w = top_dif;
            if (bot_dif >= top_dif)
            {
                final_w = bot_dif;
                dec_w = coord[i].left_bot.x;
            }

            if (left_dif >= right_dif)
            {
                final_h = left_dif;
                dec_h = coord[i].left_top.y;
            }
            dec_w = dec_w + (final_w - (final_w * 1));
            dec_h = dec_h + (final_h - (final_h * 1));
            final_h *= 1;
            final_w *= 1;
            //printf("final_h -> %d , final_w -> %d\n", final_h, final_w);
            //printf("dec_h -> %d , dec_w -> %d\n", dec_h, dec_w);
            SDL_Surface *number = SDL_CreateRGBSurface(0, final_w, final_h, 32, 0, 0, 0, 0);
            for (int j = 0; j < final_h; j++)
            {
                for (int i = 0; i < final_w; i++)
                {
                    Uint32 pix = get_pixel(image, i + dec_w, j + dec_h);
                    put_pixel(number, i, j, pix);
                }
            }
            int num_w = number->w;
            int num_h = number->h;
            num_w /= 3;
            num_h /= 3;
            int dec_n_w = 0;
            int dec_n_h = 0;
            int ct_9 = 1;
            while (ct_9 <= 9)
            {
                SDL_Surface *number_small = SDL_CreateRGBSurface(0, num_w - 50, num_h - 50, 32, 0, 0, 0, 0);
                for (int j = (dec_n_h * num_h) + 25; j < ((dec_n_h + 1) * num_h) - 25; j++)
                {
                    for (int i = (dec_n_w * num_w) + 25; i < ((dec_n_w + 1) * num_w) - 25; i++)
                    {
                        Uint32 pix = get_pixel(number, i, j);
                        put_pixel(number_small, i - ((dec_n_w * num_w) + 25), j - ((dec_n_h * num_h) + 25), pix);
                    }
                }
                
                char *str = create_str(vals[numb], abs_path);
                number_small = rescale(number_small, 28, 28);
                SDL_SaveBMP(number_small, str);
                SDL_FreeSurface(number_small);
                if ((dec_n_w + 1) % 3 == 0)
                {
                    dec_n_h++;
                    dec_n_w = 0;
                }
                else
                {
                    dec_n_w++;
                }
                ct_9++;
                numb++;
                free(str);
            }
            SDL_FreeSurface(number);
        }
        free(vals);
        free(coord);
    }
    if (ct == 4)
    {
        del /= 6;
        del *= 0.8;
        //printf("del -> %d\n", del);
        pos = 13;
        int incr = 8;
        for (; beg_lines; beg_lines = beg_lines->next)
        {
            if (beg_lines->fin == 0 && beg_lines->debut == 0)
            {
                continue;
            }
            int born_inf = (beg_lines->fin) - del;
            int born_sup = (beg_lines->fin) + del;
            //printf("pos -> %d , x-> %d , y -> %d \n",beg_lines->pos,beg_lines->debut,beg_lines->fin);
            for (int i = 0; i < w; i++)
            {
                if (born_inf < 0)
                {
                    born_inf = 0;
                }
                if (born_sup > h)
                {
                    born_sup = h;
                }
                for (int j = born_inf; j < born_sup; j++)
                {
                    Uint32 pix = get_pixel(image, i, j);
                    if (pix == SDL_MapRGB(image->format, 0, 255, 0))
                    {
                        true_list = add(true_list, i, j, pos);
                        //printf("pos -> %d\n", pos);
                        pos++;
                    }
                }
            }
            pos = pos - incr;
        }
        free_list(save_beg);
        Matrix_couple *coord = init_list_matrix_red(true_list);
        free_list(true_list);
        int numb = 0;
        int *vals = semi_elementary_cordinates();
        for (size_t i = 0; i < 19; i++)
        {
            int bot_dif = coord[i].right_bot.x - coord[i].left_bot.x;
            int top_dif = coord[i].right_top.x - coord[i].left_top.x;
            int left_dif = coord[i].left_bot.y - coord[i].left_top.y;
            int right_dif = coord[i].right_bot.y - coord[i].right_top.y;
            if (bot_dif == 0 || top_dif == 0 || left_dif == 0 || right_dif == 0)
            {
                continue;
            }
            /*
            else
            {
                printf("case: %d left_top -> (%d,%d) ,left_bot -> (%d,%d) ,right_top -> (%d,%d) ,right_bot -> (%d,%d) , \n",
                       numb,
                       coord[i].left_top.x, coord[i].left_top.y, coord[i].left_bot.x, coord[i].left_bot.y,
                       coord[i].right_top.x, coord[i].right_top.y, coord[i].right_bot.x, coord[i].right_bot.y);
            }
            */
            //printf("difs: bot -> %d ,  top-> %d , left -> %d , right -> %d\n", bot_dif, top_dif, left_dif, right_dif);
            int dec_w = coord[i].left_top.x;
            int dec_h = coord[i].right_top.y;
            int final_h = right_dif;
            int final_w = top_dif;
            if (bot_dif >= top_dif)
            {
                final_w = bot_dif;
                dec_w = coord[i].left_bot.x;
            }

            if (left_dif >= right_dif)
            {
                final_h = left_dif;
                dec_h = coord[i].left_top.y;
            }
            dec_w = dec_w + (final_w - (final_w * 0.92));
            dec_h = dec_h + (final_h - (final_h * 0.92));
            final_h *= 0.92;
            final_w *= 0.92;
            //printf("final_h -> %d , final_w -> %d\n", final_h, final_w);
            //printf("dec_h -> %d , dec_w -> %d\n", dec_h, dec_w);

            SDL_Surface *number = SDL_CreateRGBSurface(0, final_w, final_h, 32, 0, 0, 0, 0);
            for (int j = 0; j < final_h; j++)
            {
                for (int i = 0; i < final_w; i++)
                {
                    Uint32 pix = get_pixel(image, i + dec_w, j + dec_h);
                    put_pixel(number, i, j, pix);
                }
            }
            int num_w = number->w;
            int num_h = number->h;
            num_w /= 3;
            num_h /= 3;
            int dec_n_w = 0;
            int dec_n_h = 0;
            int ct_9 = 1;
            while (ct_9 <= 9)
            {
                SDL_Surface *number_small = SDL_CreateRGBSurface(0, num_w, num_h, 32, 0, 0, 0, 0);
                for (int j = dec_n_h * num_h; j < (dec_n_h + 1) * num_h; j++)
                {
                    for (int i = dec_n_w * num_w; i < (dec_n_w + 1) * num_w; i++)
                    {
                        Uint32 pix = get_pixel(number, i, j);
                        put_pixel(number_small, i - (dec_n_w * num_w), j - (dec_n_h * num_h), pix);
                    }
                }
                
                char *str = create_str(vals[numb], abs_path);
                number_small = rescale(number_small, 28, 28);
                SDL_SaveBMP(number_small, str);
                SDL_FreeSurface(number_small);
                if ((dec_n_w + 1) % 3 == 0)
                {
                    dec_n_h++;
                    dec_n_w = 0;
                }
                else
                {
                    dec_n_w++;
                }
                ct_9++;
                numb++;
            }
            SDL_FreeSurface(number);
        }
        free(vals);
        free(coord);
    }

    if (ct == 10)
    {
        del /= 18;
        del *= 0.8;
        //printf("del -> %d\n", del);
        pos = 91;
        int incr = 20;
        for (; beg_lines; beg_lines = beg_lines->next)
        {
            if (beg_lines->fin == 0 && beg_lines->debut == 0)
            {
                continue;
            }
            int born_inf = (beg_lines->fin) - del;
            int born_sup = (beg_lines->fin) + del;
            //printf("pos -> %d , x-> %d , y -> %d \n",beg_lines->pos,beg_lines->debut,beg_lines->fin);
            for (int i = 0; i < w; i++)
            {
                if (born_inf < 0)
                {
                    born_inf = 0;
                }
                if (born_sup > h)
                {
                    born_sup = h;
                }
                for (int j = born_inf; j < born_sup; j++)
                {
                    Uint32 pix = get_pixel(image, i, j);
                    if (pix == SDL_MapRGB(image->format, 0, 255, 0))
                    {
                        true_list = add(true_list, i, j, pos);
                        //printf("pos -> %d\n", pos);
                        pos++;
                    }
                }
            }
            pos = pos - incr;
        }
        free_list(save_beg);
        Matrix_couple *coord = init_list_matrix(true_list);
        free_list(true_list);
        int numb = 0;
        for (size_t i = 0; i < 90; i++)
        {
            int bot_dif = coord[i].right_bot.x - coord[i].left_bot.x;
            int top_dif = coord[i].right_top.x - coord[i].left_top.x;
            int left_dif = coord[i].left_bot.y - coord[i].left_top.y;
            int right_dif = coord[i].right_bot.y - coord[i].right_top.y;
            if (bot_dif == 0 || top_dif == 0 || left_dif == 0 || right_dif == 0)
            {
                continue;
            }
            else
            {
                /*
                printf("case: %d left_top -> (%d,%d) ,left_bot -> (%d,%d) ,right_top -> (%d,%d) ,right_bot -> (%d,%d) , \n",
                       numb,
                       coord[i].left_top.x, coord[i].left_top.y, coord[i].left_bot.x, coord[i].left_bot.y,
                       coord[i].right_top.x, coord[i].right_top.y, coord[i].right_bot.x, coord[i].right_bot.y);
                       */
                numb++;
            }
            //printf("difs: bot -> %d ,  top-> %d , left -> %d , right -> %d\n", bot_dif, top_dif, left_dif, right_dif);
            int dec_w = coord[i].left_top.x;
            int dec_h = coord[i].right_top.y;
            int final_h = right_dif;
            int final_w = top_dif;
            if (bot_dif >= top_dif)
            {
                final_w = bot_dif;
                dec_w = coord[i].left_bot.x;
            }

            if (left_dif >= right_dif)
            {
                final_h = left_dif;
                dec_h = coord[i].left_top.y;
            }

            dec_w = dec_w + (final_w - (final_w * 0.85));
            dec_h = dec_h + (final_h - (final_h * 0.85));
            final_h *= 0.7;
            final_w *= 0.7;
            //printf("final_h -> %d , final_w -> %d\n", final_h, final_w);
            //printf("dec_h -> %d , dec_w -> %d\n", dec_h, dec_w);

            char *str = create_str(numb, abs_path);
            SDL_Surface *number = SDL_CreateRGBSurface(0, final_w, final_h, 32, 0, 0, 0, 0);
            for (int j = 0; j < final_h; j++)
            {
                for (int i = 0; i < final_w; i++)
                {
                    Uint32 pix = get_pixel(image, i + dec_w, j + dec_h);
                    put_pixel(number, i, j, pix);
                }
            }
            number = rescale(number, 28, 28);
            SDL_SaveBMP(number, str);
        }
        free(coord);
    }
}

void convolve_final(SDL_Surface *hor, SDL_Surface *vert, SDL_Surface *clean)
{
    size_t w = clean->w;
    size_t h = clean->h;
    for (size_t i = 0; i < w; i++)
    {
        for (size_t j = 0; j < h; j++)
        {
            Uint32 pix = get_pixel(hor, i, j);
            Uint32 pix1 = get_pixel(vert, i, j);
            if ((pix == SDL_MapRGB(hor->format, 255, 0, 0)) && (pix1 == SDL_MapRGB(vert->format, 255, 0, 0)))
            {
                Uint32 pixel = SDL_MapRGB(clean->format, 0, 255, 0);
                put_pixel(clean, i, j, pixel);
            }
        }
    }
}

void convolve_normal(SDL_Surface *hor, SDL_Surface *clean)
{
    size_t w = clean->w;
    size_t h = clean->h;
    for (size_t i = 0; i < w; i++)
    {
        for (size_t j = 0; j < h; j++)
        {
            Uint32 pix = get_pixel(hor, i, j);
            if (pix == SDL_MapRGB(hor->format, 255, 0, 0))
            {
                Uint32 pixel = SDL_MapRGB(clean->format, 0, 255, 0);
                put_pixel(clean, i, j, pixel);
            }
        }
    }
}

void normalise(SDL_Surface *image)
{
    int w = image->w;
    int h = image->h;
    int w_area = w * 0.04;
    int h_area = h * 0.04;
    int count = 1;
    struct list *final = init_list(0, 0, 0);
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            Uint32 pix = get_pixel(image, i, j);
            //first in
            if (pix == SDL_MapRGB(image->format, 0, 255, 0))
            {
                //Run the area
                //Verify it doesnt surpass

                int border_w_x = i + w_area;
                int border_h_x = j + h_area;

                int border_w_y = i - w_area;
                int border_h_y = j - h_area;

                if (border_h_y < 0)
                {
                    border_h_y = 0;
                }

                if (border_w_y < 0)
                {
                    border_w_y = 0;
                }

                if (border_w_x > w)
                {
                    border_w_x = w;
                }

                if (border_h_x > h)
                {
                    border_h_x = h;
                }

                struct list *l = init_list(i, j, 0);
                for (int k = border_w_y; k < border_w_x; k++)
                {
                    for (int z = border_h_y; z < border_h_x; z++)
                    {
                        Uint32 pix = get_pixel(image, k, z);
                        if (pix == SDL_MapRGB(image->format, 0, 255, 0))
                        {
                            l = add(l, k, z, 0);
                            Uint32 pixel = SDL_MapRGB(image->format, 0, 0, 255);
                            put_pixel(image, k, z, pixel);
                        }
                    }
                }
                struct list *list_start = l;
                for (; l; l = l->next)
                {
                    size_t x = l->debut;
                    size_t y = l->fin;
                    int border_w_x = x + w_area;
                    int border_h_x = y + h_area;

                    int border_w_y = x - w_area;
                    int border_h_y = y - h_area;

                    if (border_h_y < 0)
                    {
                        border_h_y = 0;
                    }

                    if (border_w_y < 0)
                    {
                        border_w_y = 0;
                    }

                    if (border_w_x > w)
                    {
                        border_w_x = w;
                    }

                    if (border_h_x > h)
                    {
                        border_h_x = h;
                    }
                    for (int k = border_w_y; k < border_w_x; k++)
                    {
                        for (int z = border_h_y; z < border_h_x; z++)
                        {
                            Uint32 pix = get_pixel(image, k, z);
                            if (pix == SDL_MapRGB(image->format, 0, 255, 0))
                            {
                                list_start = add(list_start, k, z, 0);
                                Uint32 pixel = SDL_MapRGB(image->format, 0, 0, 255);
                                put_pixel(image, k, z, pixel);
                            }
                        }
                    }
                }
                size_t list_length = list_len(list_start);
                size_t *sum = malloc(sizeof(size_t) * 2);
                sum = list_sum(list_start);
                size_t new_x = sum[0] / list_length;
                size_t new_y = sum[1] / list_length;
                Uint32 pix = SDL_MapRGB(image->format, 255, 0, 0);
                put_pixel(image, new_x, new_y, pix);
                free(sum);
                free_list(list_start);
                final = add(final, new_x, new_y, count);
                count++;
            }
        }
    }
    free_list(final);
}

void apply_solve(char *argv)
{
    char main_dir[PATH_MAX];
    directory(argv,main_dir);
    SDL_Surface *image_0 = SDL_LoadBMP(argv);
    SDL_Surface *image_3 = SDL_LoadBMP(argv);
    SDL_Surface *image_2 = SDL_LoadBMP(argv);

    grey_scale(image_0);
    //Gauss
    image_0 = gaussian_blur(image_0);
    char gauss[PATH_MAX];
    save_in_dir(main_dir,"/gauss.bmp",gauss);
    SDL_SaveBMP(image_0, gauss);
    
    histogram_equil(image_0);
    histogram_spreading(image_0);
    double thr = big_hist(image_0);
    thresholding(image_0, thr);

    image_0 = hysteris(image_0);
    
    image_0 = sobel(image_0);

    histogram_equil(image_0);
    
    char coloured[PATH_MAX];
    save_in_dir(main_dir,"/coloured.bmp",coloured);

    char rot[PATH_MAX];
    save_in_dir(main_dir,"/rot.bmp",rot);
    
    
    image_0 = hough_transform(image_0, image_2,coloured,rot);
    image_2 = SDL_LoadBMP(rot);
    
    
    grey_scale(image_2);
    image_2 = gaussian_blur(image_2);

    histogram_spreading(image_2);
    histogram_equil(image_2);

    thresholding(image_2, thr);

    image_2 = hysteris(image_2);

    char hyst[PATH_MAX];
    save_in_dir(main_dir,"/hyst.bmp",hyst);

    SDL_SaveBMP(image_2, hyst);
    SDL_Surface *image_1 = SDL_LoadBMP(coloured);

    image_0 = crop_the_image(image_2, image_1);
    
    image_0 = sobel(image_0);

    histogram_equil(image_0);
    image_2 = SDL_LoadBMP(hyst);

    
    
    image_0 = hough_transform(image_0, image_2,coloured,rot);

    char hysteris_save[PATH_MAX];
    save_in_dir(main_dir,"/hysteris.bmp",hysteris_save);

    
    SDL_SaveBMP(image_0, hysteris_save);
    image_3 = SDL_LoadBMP(hysteris_save);
    SDL_Surface *image_4 = SDL_LoadBMP(hysteris_save);
    SDL_Surface *image_5 = SDL_LoadBMP(hysteris_save);

    image_0 = vertical_edge_detection_sobel(image_0);
    thresholding(image_0, 0.3);
    image_0 = hysteris(image_0);
    image_0 = hysteris(image_0);
    image_0 = hysteris(image_0);
    image_0 = vertical_edge_detection_sobel(image_0);
    histogram_equil(image_0);
    image_0 = rotation_90(image_0);
    hough_alligned(image_0, 0.45);
    image_0 = rotation_270(image_0);

    image_3 = horizontal_edge_detection_sobel(image_3);
    thresholding(image_3, 0.3);
    image_3 = hysteris(image_3);
    image_3 = hysteris(image_3);
    image_3 = hysteris(image_3);
    image_3 = horizontal_edge_detection_sobel(image_3);
    histogram_equil(image_3);
    hough_alligned(image_3, 0.45);

    convolve_final(image_0, image_3, image_4);
    normalise(image_4);

    convolve_normal(image_4, image_5);
    segmenting(image_5,argv);
    SDL_FreeSurface(image_5);
    
}
