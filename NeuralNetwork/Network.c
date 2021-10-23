//
// Created by Yves-Antoine on 23/10/2021.
//

#include <stdlib.h>
#include <stdio.h>

typedef struct Neuron{
    float bias;
    float delta_bias;

    float z;
    float delta_z;

    float activation;
    float delta_activation;

    float * weights;
    float * delta_weights;
} Neuron;

Neuron * create_neuron(size_t num_weights, size_t num_out_weights){
    Neuron * neuron = (Neuron *) malloc(sizeof (Neuron));

    float * weights = malloc(sizeof (float) * num_weights);
    for(size_t weight_index = 0; weight_index < num_weights; weight_index++)
        weights[weight_index] = ((float)(rand() % 10) )/ 10.f;  // NOLINT(cert-msc50-cpp)

    neuron -> bias = ((float)(rand() % 10) )/ 10.f;  // NOLINT(cert-msc50-cpp)
    neuron -> delta_bias = 0.f;

    neuron -> z = 0.f;
    neuron -> delta_z = 0.f;

    neuron -> activation = 0.f;
    neuron -> delta_activation = 0.f;

    neuron -> weights = weights;
    neuron -> delta_weights = (float*) calloc(num_out_weights,  sizeof(float));

    return neuron;
} // create_neuron

typedef struct Layer{
    size_t size;
    Neuron ** neurons;
}Layer;

Layer * create_layer(size_t size, size_t prev_size, size_t next_size){
    Layer * layer = (Layer *) malloc(sizeof (Layer));

    Neuron ** neurons = malloc(sizeof (Neuron) * size);
    for(size_t neuron_index = 0; neuron_index < size ; neuron_index++)
        neurons[neuron_index] = create_neuron(prev_size, next_size);

    layer -> size = size;
    layer -> neurons = neurons;

    return layer;
} // create_layer

typedef struct Network{
    size_t num_layers;
    Layer ** layers;
}Network;

Network * create_network(const size_t sizes[], size_t num_layers){
    Network * network = (Network *) malloc(sizeof(Network));

    Layer ** layers = malloc(sizeof (Layer) * num_layers);
    size_t prev = 0;
    for(size_t layer_index = 0 ; layer_index < num_layers - 1; layer_index++){
        layers[layer_index] = create_layer(sizes[layer_index], prev, sizes[layer_index + 1]);
        prev = sizes[layer_index];
    }
    layers[num_layers - 1] = create_layer(sizes[num_layers - 1], prev, 0);

    network -> num_layers = num_layers;
    network -> layers = layers;

    return network;
} // create_network

void delete_network(Network * network){
    for(size_t layer_index = 0; layer_index < network -> num_layers; layer_index++){
        Layer * layer = network -> layers[layer_index];
        for(size_t neuron_index = 0;neuron_index < layer -> size; neuron_index++){
            Neuron * neuron = layer -> neurons[neuron_index];
            free(neuron -> weights);
            free(neuron -> delta_weights);
            free(neuron);
        }
        free(layer -> neurons);
        free(layer);
    }
    free (network -> layers);
    free (network);
}

void print_network(Network* network){
    size_t num_layer = network -> num_layers;
    printf("Network ( (length : %zu)", num_layer);

    for (size_t layer_index = 0; layer_index < num_layer; layer_index++){
        Layer * layer = network -> layers[layer_index];
        size_t num_weight = layer_index ? network -> layers[layer_index - 1] -> size : 0;
        size_t size = layer -> size;
        printf("\n    Layer [ (size : %zu)", size);

        for (size_t neuron_index = 0; neuron_index < size; neuron_index++){
            Neuron * neuron = layer -> neurons[neuron_index];

            printf("\n        Neuron { ");
            if (num_weight){
                printf("(bias : %f - links : %zu)\n                <\n                    %f",
                       neuron -> bias, num_weight, neuron -> weights[0]);

                for (size_t weight_index = 1; weight_index < num_weight; weight_index++)
                    printf(",\n                    %f", neuron -> weights[weight_index]);

                printf("\n                >\n       ");
            } else
                printf("INPUT");
            printf(" }");
        } printf("\n    ]");
    } printf("\n)\n");
} // print_network
