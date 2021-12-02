//
// Created by yvesantoine on 01/12/2021.
//

#include "Network.h"
#include "NetworkTraining.h"
#include "Image.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include<time.h>


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
    float ** input_data = malloc(sizeof (float*) * image_count);
    float ** output_data = malloc(sizeof (float*) * image_count);

    load_data(input_data, output_data, path);

    train_neural_network(network, input_data, output_data, image_count, 100, 0.05f);

    for (int index = 0; index < image_count; index++) {
        free(input_data[index]);
        free(output_data[index]);
    }

    free(input_data);
    free(output_data);
}

static inline int get_max_output(float * output_data)
{
    int max_index = 0;
    float max_value = output_data[0];
    for(int index = 1; index < 10; index++)
        if (output_data[index] > max_value){
            max_index = index;
            max_value = output_data[index];
        }
    return max_index;
}
void test_network_image(Network * network)
{
    const char path[] = "./ImageDataBase/data/";

    int image_count = count_files_in_directory(path);

    float ** input_data = malloc(sizeof (float*) * image_count);
    float ** output_data = malloc(sizeof (float*) * image_count);

    load_data(input_data, output_data, path);

    srand(time(0));
    int score = 0;
    for (int i = 0; i < image_count; i++){
        //size_t random_index = rand() % image_count;
        float * test_data = input_data[i];
        float * test_expected = output_data[i];

        float * test_output = feed_forward(network, test_data);

        for (int j = 0; j < 10; j++) printf("%f ", test_output[j]);
        printf("\n");

        int result = get_max_output(test_output);
        int expected_output = get_max_output(test_expected);
        //printf("Got %d and expected %d\n", result, expected_output);
        score += result == expected_output;
        // printf("result %d\n", result);
        free(test_output);
    }
    printf("Score %d / %d\n", score, image_count);

    for (int index = 0; index < image_count; index++) {
        free(input_data[index]);
        free(output_data[index]);
    }

    free(input_data);
    free(output_data);
}

void infinite_train(Network * network, const char * save_path){
    const char path[] = "./ImageDataBase/data/";

    int image_count = count_files_in_directory(path);
    float ** input_data = malloc(sizeof (float*) * image_count);
    float ** output_data = malloc(sizeof (float*) * image_count);

    load_data(input_data, output_data, path);

    while(1){
        train_neural_network(network, input_data, output_data, image_count, 100, 5.f);
        printf("Saving... Don't quit...\n");
        save_network(network, save_path);
        printf("Saved.\n");
        test_network_image(network);
    }
}

