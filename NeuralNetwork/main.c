//
// Created by Yves-Antoine on 29/09/2021.
//

#include <stdio.h>
#include <malloc.h>
#include "Network.h"


void train_for_xor(Network * network){
    size_t size_of_float = sizeof (float);
    #define TRAINING_DATA_SIZE 4

    float * input_data[TRAINING_DATA_SIZE] = {
            malloc(size_of_float * 2),
            malloc(size_of_float * 2),
            malloc(size_of_float * 2),
            malloc(size_of_float * 2),
    };
    input_data[0][0] = 0.f; input_data[0][1] = 0.f;
    input_data[1][0] = 1.f; input_data[1][1] = 0.f;
    input_data[2][0] = 0.f; input_data[2][1] = 1.f;
    input_data[3][0] = 1.f; input_data[3][1] = 1.f;

    float * expected_output[TRAINING_DATA_SIZE] = {
            malloc(size_of_float * 2),
            malloc(size_of_float * 2),
            malloc(size_of_float * 2),
            malloc(size_of_float * 2)
    };
    expected_output[0][0] = 1.f; expected_output[0][1] = 0.f;
    expected_output[1][0] = 0.f; expected_output[1][1] = 1.f;
    expected_output[2][0] = 0.f; expected_output[2][1] = 1.f;
    expected_output[3][0] = 1.f; expected_output[3][1] = 0.f;

    train_network(network, input_data, expected_output, TRAINING_DATA_SIZE, 10000, 4, 0.5f);

    for(size_t index = 0; index < TRAINING_DATA_SIZE; index++){free(input_data[index]); free(expected_output[index]);}
}

void print_output(float * output, size_t size){
    for(size_t i = 0; i < size; i++)
        printf("| %f ",output[i]);
    printf("\n");
}

int main(){

    #define NUM_LAYER 3
    size_t sizes[NUM_LAYER] = {2,2,2};
    Network * network = create_network(sizes, NUM_LAYER);

    train_for_xor(network);


    {
        float inputs[] = {1.f, 1.f};
        float * test_output = feed_forward(network, inputs);
        print_output(test_output, network -> layers[network -> num_layers - 1] -> size);
    }
    {
        float inputs[] = {0.f, 1.f};
        float * test_output = feed_forward(network, inputs);
        print_output(test_output, network -> layers[network -> num_layers - 1] -> size);
    }
    {
        float inputs[] = {1.f, 0.f};
        float * test_output = feed_forward(network, inputs);
        print_output(test_output, network -> layers[network -> num_layers - 1] -> size);
    }
    {
        float inputs[] = {0.f, 0.f};
        float * test_output = feed_forward(network, inputs);
        print_output(test_output, network -> layers[network -> num_layers - 1] -> size);
    }


    print_network(network);

    return 0;
}
