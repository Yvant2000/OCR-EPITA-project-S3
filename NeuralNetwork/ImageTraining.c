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

static inline double * int_to_data_array(int digit)
{
    double * data = calloc(10, sizeof (double));
    data[digit] = 1.;
    return data;
}

static inline void load_data(double ** input_data, double ** output_data, const char * path)
{
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
}

void train_for_image(Network * network) {
    /*
    ** Trains a Network to recognize digit on image
     */

    const char path[] = "./ImageDataBase/data/";

    int image_count = count_files_in_directory(path);
    double ** input_data = malloc(sizeof (double*) * image_count);
    double ** output_data = malloc(sizeof (double*) * image_count);

    load_data(input_data, output_data, path);

    train_neural_network(network, input_data, output_data,
                         image_count, 1000, 0.5);

    for (int index = 0; index < image_count; index++) {
        free(input_data[index]);
        free(output_data[index]);
    }

    free(input_data);
    free(output_data);
}

static inline int get_max_output(double * output_data)
{
    int max_index = 0;
    double max_value = output_data[0];
    for (int index = 1; index < 10; index++) {
        if (output_data[index] > max_value){
            max_index = index;
            max_value = output_data[index];
        }
    }

    return max_index;
}
void test_network_image(Network * network)
{
    //const char path[] = "./ImageDataBase/data/";
    const char path[] = "./ImageDataBase/numbers_train/";

    int image_count = count_files_in_directory(path);

    double ** input_data = malloc(sizeof (double*) * image_count);
    double ** output_data = malloc(sizeof (double*) * image_count);

    load_data(input_data, output_data, path);

    srand(time(0));
    int score = 0;
    for (int i = 0; i < image_count; i++){
        //size_t random_index = rand() % image_count;
        double * test_data = input_data[i];
        double * test_expected = output_data[i];

        double * test_output = feed_forward(network, test_data);

//        for (int j = 0; j < 10; j++)
//            printf("%lf ", test_output[j]);
//        printf("\n");

        int result = get_max_output(test_output);
        int expected_output = get_max_output(test_expected);
        //printf("Got %d and expected %d\n", result, expected_output);
        if (result == expected_output)
            score++;
        else
            printf("result %d  but expected %d\n", result, expected_output);
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
    //const char path[] = "./ImageDataBase/data/";
    const char path[] = "./ImageDataBase/numbers_train/";

    int image_count = count_files_in_directory(path);
    double ** input_data = malloc(sizeof (double*) * image_count);
    double ** output_data = malloc(sizeof (double*) * image_count);

    load_data(input_data, output_data, path);

    while(1){
        train_neural_network(network, input_data,
                             output_data,
                             image_count,
                             1000,
                             0.2);
        printf("Saving... Don't quit...\n");
        save_network(network, save_path);
        printf("Saved.\n");
        test_network_image(network);
    }
}

double * image_to_recognized_array(Network * network, const char * image_path)
{
    double * input_data =  image_to_array(load_image(image_path));
    double * test_output = feed_forward(network, input_data);
    free(input_data);
    return test_output;
}



int image_to_digit(Network * network, const char * image_path)
{
    double * test_output = image_to_recognized_array(network, image_path);
    int result = get_max_output(test_output);

    free(test_output);
    return result;
}