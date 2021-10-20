//
// Created by Yves-Antoine on 29/09/2021.
//

#include <stdio.h>
#include <malloc.h>
#include "Network.h"


void train_for_xor(Network * network){
    size_t size_of_float = sizeof (float);
    const size_t training_data_size = 4;

    float * input_data[4] = {
            malloc(size_of_float * 2),
            malloc(size_of_float * 2),
            malloc(size_of_float * 2),
            malloc(size_of_float * 2),
    };
    input_data[0][0] = 0.f; input_data[0][1] = 0.f;
    input_data[1][0] = 1.f; input_data[1][1] = 0.f;
    input_data[2][0] = 0.f; input_data[2][1] = 1.f;
    input_data[3][0] = 1.f; input_data[3][1] = 1.f;

    float * expected_output[4] = {
            malloc(size_of_float),
            malloc(size_of_float),
            malloc(size_of_float),
            malloc(size_of_float)
    };
    expected_output[0][0] = 0.f;
    expected_output[1][0] = 1.f;
    expected_output[2][0] = 1.f;
    expected_output[3][0] = 0.f;

    train_network(network, input_data, expected_output, training_data_size, 30, 2, 0.3f);
}



int main(){

    size_t sizes[] = {2,3,1};
    Network * network = create_network(sizes, 3);

    train_for_xor(network);

    printf("\n");

    float inputs[] = {1.f, 1.f};
    float * test_output = feed_forward(network, inputs);

     for(int i = 0;i < network -> layers[network -> num_layers - 1] -> size;i++)
        printf("%f\n",test_output[i]);
     printf("\n");

     print_network(network);

    return 0;
}
