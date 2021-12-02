//
// Created by yvesantoine on 01/12/2021.
//

#include "Network.h"
#include "NetworkTraining.h"
#include "Image.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>

static inline int count_files_in_directory(const char * path)
{
    int file_count = 0;
    DIR * directory_ptr;
    struct dirent * entry;

    directory_ptr = opendir(path); /* There should be error handling after this */
    while ((entry = readdir(directory_ptr)) != NULL)
        if (entry -> d_type == DT_REG) /* If the entry is a regular file */
            file_count++;
    closedir(directory_ptr);

    return file_count;
}

static inline int extract_digit(char * name)
{
    size_t length = strlen(name);
    return name[length - 5] - (int)'0';
}

static inline float * int_to_data_array(int digit)
{
    float * data = calloc(10, sizeof (float));
    data[digit] = 1.f;
    return data;
}

static inline void load_data(float ** input_data, float ** output_data, const char * path)
{
    image_init();
    DIR *d = opendir(path);
    struct dirent *dir;
    size_t index = 0;
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            size_t length = strlen(dir -> d_name);
            if (length == 1)
                continue;
            if (length == 2)
                continue;

            char str[256];
            strcpy(str, path);
            strcat(str, dir->d_name);
            input_data[index] = image_to_array(load_image(str));
            output_data[index] = int_to_data_array(extract_digit(dir -> d_name));

            index++;
        }
        closedir(d);
    }
    image_quit();
}

void train_for_image(Network * network) {
    /*
    ** Trains a Network to recognize digit on image
     */

    const char path[] = "./ImageDataBase/data/";

    int image_count = count_files_in_directory(path);
    printf("found %d images in directory\n", image_count);
    float ** input_data = malloc(sizeof (float*) * image_count);
    float ** output_data = malloc(sizeof (float*) * image_count);

    load_data(input_data, output_data, path);

    train_neural_network(network, input_data, output_data, image_count, 30, 1.f);

    for (int index = 0; index < image_count; index++) {
        free(input_data[index]);
        free(output_data[index]);
    }

    free(input_data);
    free(output_data);


}
