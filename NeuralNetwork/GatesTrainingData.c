//
// Created by Yves-Antoine on 23/10/2021.
//

#include "Network.h"
#include "NetworkTraining.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>


void train_for_xor(Network * network) {
    /*
    ** Trains a Network to act like a XOR Gate
     */

    size_t size_of_float = sizeof(double);

    #define XOR_TRAINING_DATA_SIZE 4

    double *input_data[XOR_TRAINING_DATA_SIZE] = {
            malloc(size_of_float * 2),
            malloc(size_of_float * 2),
            malloc(size_of_float * 2),
            malloc(size_of_float * 2),
    };
    input_data[0][0] = 0.f;input_data[0][1] = 0.f;
    input_data[1][0] = 1.f;input_data[1][1] = 0.f;
    input_data[2][0] = 0.f;input_data[2][1] = 1.f;
    input_data[3][0] = 1.f;input_data[3][1] = 1.f;

    double *expected_output[XOR_TRAINING_DATA_SIZE] = {
            malloc(size_of_float * 1),
            malloc(size_of_float * 1),
            malloc(size_of_float * 1),
            malloc(size_of_float * 1)
    };
    expected_output[0][0] = 0.f;
    expected_output[1][0] = 1.f;
    expected_output[2][0] = 1.f;
    expected_output[3][0] = 0.f;

    train_neural_network(network, input_data, expected_output, XOR_TRAINING_DATA_SIZE, 100000, 0.4);

    for (size_t index = 0; index < XOR_TRAINING_DATA_SIZE; index++) {
        free(input_data[index]);
        free(expected_output[index]);
    }
}


void train_for_or(Network * network) {
    /*
    ** Trains a Network to act like a XOR Gate
     */

    size_t size_of_float = sizeof(double);

    #define OR_TRAINING_DATA_SIZE 4

    double *input_data[OR_TRAINING_DATA_SIZE] = {
            malloc(size_of_float * 2),
            malloc(size_of_float * 2),
            malloc(size_of_float * 2),
            malloc(size_of_float * 2),
    };
    input_data[0][0] = 0.f;input_data[0][1] = 0.f;
    input_data[1][0] = 1.f;input_data[1][1] = 0.f;
    input_data[2][0] = 0.f;input_data[2][1] = 1.f;
    input_data[3][0] = 1.f;input_data[3][1] = 1.f;

    double *expected_output[OR_TRAINING_DATA_SIZE] = {
            malloc(size_of_float * 1),
            malloc(size_of_float * 1),
            malloc(size_of_float * 1),
            malloc(size_of_float * 1)
    };
    expected_output[0][0] = 0.f;
    expected_output[1][0] = 1.f;
    expected_output[2][0] = 1.f;
    expected_output[3][0] = 1.f;

    train_neural_network(network, input_data, expected_output, OR_TRAINING_DATA_SIZE, 100000, 0.05f);

    for (size_t index = 0; index < OR_TRAINING_DATA_SIZE; index++) {
        free(input_data[index]);
        free(expected_output[index]);
    }
}



void train_for_and(Network * network) {
    /*
    ** Trains a Network to act like a XOR Gate
     */

    size_t size_of_float = sizeof(double);

    #define AND_TRAINING_DATA_SIZE 4

    double *input_data[AND_TRAINING_DATA_SIZE] = {
            malloc(size_of_float * 2),
            malloc(size_of_float * 2),
            malloc(size_of_float * 2),
            malloc(size_of_float * 2),
    };
    input_data[0][0] = 0.f;input_data[0][1] = 0.f;
    input_data[1][0] = 1.f;input_data[1][1] = 0.f;
    input_data[2][0] = 0.f;input_data[2][1] = 1.f;
    input_data[3][0] = 1.f;input_data[3][1] = 1.f;

    double *expected_output[AND_TRAINING_DATA_SIZE] = {
            malloc(size_of_float * 1),
            malloc(size_of_float * 1),
            malloc(size_of_float * 1),
            malloc(size_of_float * 1)
    };
    expected_output[0][0] = 0.f;
    expected_output[1][0] = 0.f;
    expected_output[2][0] = 0.f;
    expected_output[3][0] = 1.f;

    train_neural_network(network, input_data, expected_output, AND_TRAINING_DATA_SIZE, 100000, 0.05f);

    for (size_t index = 0; index < AND_TRAINING_DATA_SIZE; index++) {
        free(input_data[index]);
        free(expected_output[index]);
    }
}



void train_for_nand(Network * network) {
    /*
    ** Trains a Network to act like a XOR Gate
     */

    size_t size_of_float = sizeof(double);

#define NAND_TRAINING_DATA_SIZE 4

    double *input_data[NAND_TRAINING_DATA_SIZE] = {
            malloc(size_of_float * 2),
            malloc(size_of_float * 2),
            malloc(size_of_float * 2),
            malloc(size_of_float * 2),
    };
    input_data[0][0] = 0.f;input_data[0][1] = 0.f;
    input_data[1][0] = 1.f;input_data[1][1] = 0.f;
    input_data[2][0] = 0.f;input_data[2][1] = 1.f;
    input_data[3][0] = 1.f;input_data[3][1] = 1.f;

    double *expected_output[NAND_TRAINING_DATA_SIZE] = {
            malloc(size_of_float * 1),
            malloc(size_of_float * 1),
            malloc(size_of_float * 1),
            malloc(size_of_float * 1)
    };
    expected_output[0][0] = 1.f;
    expected_output[1][0] = 1.f;
    expected_output[2][0] = 1.f;
    expected_output[3][0] = 0.f;

    train_neural_network(network, input_data, expected_output, NAND_TRAINING_DATA_SIZE, 1000, 0.05);

    for (size_t index = 0; index < NAND_TRAINING_DATA_SIZE; index++) {
        free(input_data[index]);
        free(expected_output[index]);
    }
}

void print_output(double * output, size_t size){
    for(size_t i = 0; i < size; i++)
        printf("| %f ",output[i]);
    printf("\n");
}

void test_inputs_gate(Network * network){
    {
        printf("0 0 ");
        double inputs[] = {0.f, 0.f};
        double * test_output = feed_forward(network, inputs);
        print_output(test_output, network -> layers[network -> num_layers - 1] -> size);
        free(test_output);
    }
    {
        printf("0 1 ");
        double inputs[] = {0.f, 1.f};
        double * test_output = feed_forward(network, inputs);
        print_output(test_output, network -> layers[network -> num_layers - 1] -> size);
        free(test_output);
    }
    {
        printf("1 0 ");
        double inputs[] = {1.f, 0.f};
        double * test_output = feed_forward(network, inputs);
        print_output(test_output, network -> layers[network -> num_layers - 1] -> size);
        free(test_output);
    }
    {
        printf("1 1 ");
        double inputs[] = {1.f, 1.f};
        double * test_output = feed_forward(network, inputs);
        print_output(test_output, network -> layers[network -> num_layers - 1] -> size);
        free(test_output);
    }
}