#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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



int thresholding(SDL_Surface* image, double t)
{
    int w = image->w;
    int h = image->h;
    //bidouiller t si c'est pas précis, 0.4 pour l'image 4, .15 pour le reste
    //double t = 0.15;
    int s = fmax(w,h)/16;
    Uint32 *intImg = malloc(sizeof(Uint32)*w*h);
    for(int i = 0; i < w; i++)
    {
        int sum = 0;
        for(int j = 0; j < h; j++)
        {
            Uint8 pixel = pixel_grey(image,i,j);
            sum += pixel;
            if(i==0)
            {
                intImg[i*h+j] = sum;
            }
            else
            {
                intImg[i*h+j] = intImg[(i-1)*h+j]+ sum;
            }
        }
    }
    for(int i = 0; i <  w; i++)
    {
        for(int j = 0; j <  h; j++)
        {
            int x1 = fmax(i-s,1);
            int x2 = fmin(i+s,w-1);
            int y1 = fmax(j-s,1);
            int y2 = fmin(j+s,h-1);
            int count = (x2-x1)*(y2-y1);
            double sum = intImg[x2*h+y2]-intImg[x2*h+(y1-1)]-intImg[(x1-1)*h+y2]+intImg[(x1-1)*h+(y1-1)];
            Uint8 pixel = pixel_grey(image, i, j);
	    if(sum * (1.0 - t) < (pixel * count)){
		    pixel = 0;
	    }
            else
            {
                pixel = 255;
            }
	    Uint32 pix = SDL_MapRGB(image->format,pixel,pixel,pixel);
            put_pixel(image, i, j, pix);
        }
    }
    free(intImg);
    return 0;
}




int thresholding_decompose(SDL_Surface* image, double t, int p_w,int p_h,int n)
{
    int w = n;
    int h = n;
    //bidouiller t si c'est pas précis, 0.4 pour l'image 4, .15 pour le reste
    //double t = 0.15;
    int s = fmax(w,h)/16;
    Uint32 *intImg = malloc(sizeof(Uint32)*w*h);
    for(int i = p_w; i < p_w+n; i++)
    {
        int sum = 0;
        for(int j = p_h; j < p_h+n; j++)
        {
            Uint8 pixel = pixel_grey(image,i,j);
            sum += pixel;
            if(i==0)
            {
                intImg[i*h+j] = sum;
            }
            else
            {
                intImg[i*h+j] = intImg[(i-1)*h+j]+ sum;
            }
        }
    }
    for(int i = p_w; i < p_w+n; i++)
    {
        for(int j = p_h; j < p_h+n; j++)
        {
            int x1 = fmax(i-s,1);
            int x2 = fmin(i+s,w-1);
            int y1 = fmax(j-s,1);
            int y2 = fmin(j+s,h-1);
            int count = (x2-x1)*(y2-y1);
            double sum = intImg[x2*h+y2]-intImg[x2*h+(y1-1)]-intImg[(x1-1)*h+y2]+intImg[(x1-1)*h+(y1-1)];
            Uint8 pixel = pixel_grey(image, i, j);
	    if(sum * (1.0 - t) < (pixel * count)){
		    pixel = 0;
	    }
            else
            {
                pixel = 255;
            }
	    Uint32 pix = SDL_MapRGB(image->format,pixel,pixel,pixel);
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
    double *histogramm = calloc(256,sizeof(double));
    for (int i = 0; i < w; i++)
    {   
        for (int j = 0; j < h; j++)
        {
            Uint8 intensity = pixel_grey(image,i,j);
            histogramm[intensity]+=1.;
        }
    }
    return histogramm;
}

double cumulative_histogram_rec(double *hist,int i,double div){
    if(i==0){
        hist[i] = (hist[i]*255.)/div;
        return hist[i];
    }
    else{
        hist[i] = (hist[i]*255.)/div;
        hist[i] += cumulative_histogram_rec(hist,i-1,div);
        return hist[i];
    }
}

double *cumulative_histogram(SDL_Surface *image){
    int w = image->w;
    int h = image->h;
    double div = (double)(w*h);
    double *hist = histogram(image);
    cumulative_histogram_rec(hist,255,div);
    return hist;
}



void histogram_spreading(SDL_Surface *image)
{
    int w = image->w;
    int h = image->h;
    double *hist = histogram(image);
    int i =0;
    while (hist[i]==0. && i<256)
    {
        i++;
    }
    int min_intensity = i;
    int max_intensity = i;
    while (i<256)
    {
        if(hist[i]!=0.){
            max_intensity = i;
        }
        i++;
    }
    for (int i = 0; i < w; i++)
    {   
        for (int j = 0; j < h; j++)
        {
            Uint8 intensity = pixel_grey(image,i,j);
            Uint8 new_intensity = (Uint8)(((double)(intensity - min_intensity)/(max_intensity - min_intensity))*255.);
            //printf("intensity ->%d\n",new_intensity);
            Uint32 new_pix = SDL_MapRGB(image->format,new_intensity,new_intensity,new_intensity);
            put_pixel(image,i,j,new_pix);
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
            Uint8 intensity = pixel_grey(image,i,j);
            Uint8 new_intensity = (Uint8)(hist[intensity]);
            //printf("intensity ->%d\n",new_intensity);
            Uint32 new_pix = SDL_MapRGB(image->format,new_intensity,new_intensity,new_intensity);
            put_pixel(image,i,j,new_pix);
        }
    }
    free(hist); 
}

double *histogram_decompose(SDL_Surface *image,int p_w,int p_h,int n)
{
    double *histogramm = calloc(256,sizeof(double));
    for (int i = p_w; i < p_w+n; i++)
    {   
        for (int j = p_h; j <p_h+n; j++)
        {
            Uint8 intensity = pixel_grey(image,i,j);
            histogramm[intensity]+=1.;
        }
    }

    return histogramm;
}


void histogram_spreading_decompose(SDL_Surface *image,int p_w,int p_h,int n)
{
    double *hist = histogram_decompose(image,p_w,p_h,n);
    int i =0;
    while (hist[i]==0. && i<256)
    {
        i++;
    }
    int min_intensity = i;
    int max_intensity = i;
    while (i<256)
    {
        if(hist[i]!=0.){
            max_intensity = i;
        }
        i++;
    }
    for (int i = p_w; i < p_w+n; i++)
    {   
        for (int j = p_h; j < p_h+n; j++)
        {
            Uint8 intensity = pixel_grey(image,i,j);
            Uint8 new_intensity = (Uint8)(((double)(intensity - min_intensity)/(max_intensity - min_intensity))*255.);
            //printf("intensity ->%d\n",new_intensity);
            Uint32 new_pix = SDL_MapRGB(image->format,new_intensity,new_intensity,new_intensity);
            put_pixel(image,i,j,new_pix);
        }
    }
    free(hist);
}

double *cumulative_histogram_decompose(SDL_Surface *image,int p_w,int p_h,int n){
    double div = (double)(n*n);
    double *hist = histogram_decompose(image,p_w,p_h,n);
    cumulative_histogram_rec(hist,255,div);
    return hist;
}

void histogram_equil_decompose(SDL_Surface *image,int p_w,int p_h,int n)
{
    double *hist = cumulative_histogram_decompose(image,p_w,p_h,n);
    for (int i = p_w; i < p_w+n; i++)
    {   
        for (int j = p_h; j <p_h+n; j++)
        {
            Uint8 intensity = pixel_grey(image,i,j);
            Uint8 new_intensity = (Uint8)(hist[intensity]);
            //printf("intensity ->%d\n",new_intensity);
            Uint32 new_pix = SDL_MapRGB(image->format,new_intensity,new_intensity,new_intensity);
            put_pixel(image,i,j,new_pix);
        }
    }
    free(hist); 
}

void decompose(SDL_Surface *image, int n)
{
    int w = image->w;
    int h = image->h;
    int max_run_length_w = w - (w%n) - n ;
    int max_run_length_h = h - (h%n) - n ;
    //printf("max -> %d \n",max_run_length_h);
    int p_w = 0;
    int p_h = 0;
    while(p_h <= max_run_length_h )
    {
        
        if(p_w > max_run_length_w)
        {
            p_h += n;
            p_w = 0;
            if(p_h <= max_run_length_h)
            {
                //printf(" inside p_w -> %d, p_h -> %d\n",p_w,p_h);
                //histogram_equil_decompose(image,p_w,p_h,n);
                histogram_spreading_decompose(image,p_w,p_h,n);
                histogram_equil_decompose(image,p_w,p_h,n);
                histogram_spreading_decompose(image,p_w,p_h,n);
                //thresholding_decompose(image,0.2,p_w,p_h,n);
                
            }
            //Call the function
        }
        else
        {
            //printf("p_w -> %d, p_h -> %d\n",p_w,p_h);
            //Call the function 
            //histogram_equil_decompose(image,p_w,p_h,n);
            histogram_spreading_decompose(image,p_w,p_h,n);
            histogram_equil_decompose(image,p_w,p_h,n);
            histogram_spreading_decompose(image,p_w,p_h,n);
            //thresholding_decompose(image,0.2,p_w,p_h,n);
        }
        p_w += n;
    }
}
/*
void decompose_test(int w,int h, int n)
{
    int max_run_length_w = w - (w%n) - n ;
    int max_run_length_h = h - (h%n) - n ;
    //num_w is the number of times I get the length n in w
    int num_w = max_run_length_w /n;
    //num_h is the number of times I get the length n in h
    int num_h = max_run_length_h/n;
    printf("max -> %d \n",max_run_length_h);
    int p_w = 0;
    int p_h = 0;
    while(p_h < max_run_length_h )
    {
        
        if(p_w >= max_run_length_w)
        {
            p_h += n;
            p_w = 0;
            if(p_h <= max_run_length_h)
            {
                printf(" inside p_w -> %d, p_h -> %d\n",p_w,p_h);
            }
            //Call the function
        }
        else
        {
            printf("p_w -> %d, p_h -> %d\n",p_w,p_h);
            //Call the function 
        }
        p_w += n;
        //histogram_equil_decompose(image,p_w,p_h,n);
    }
}
*/