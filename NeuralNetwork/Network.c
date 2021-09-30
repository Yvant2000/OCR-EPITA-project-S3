//
// Created by Yves-Antoine on 29/09/2021.
//

# include <stdlib.h>
# include <math.h>
#include <stdio.h>


typedef struct Neuron{
    float bias;
    short num_weight;
    float * weights; // weights length is num_weight;
}Neuron;

Neuron * create_neuron(short num_weights){
    Neuron * neuron = (Neuron *) malloc(sizeof (Neuron));

    float * weights = malloc(sizeof (float) * num_weights);
    for(short weight_index = 0; weight_index < num_weights; weight_index++){
        weights[weight_index] = 0.5f;
    }

    neuron -> bias = 0.5f;
    neuron -> num_weight = num_weights;
    neuron -> weights = weights;

    return neuron;
}


typedef struct Layer{
    short size;
    Neuron ** neurons; // neurons length is size
}Layer;

Layer * create_layer(short size, short prev_size){
    Layer * layer = (Layer *) malloc(sizeof (Layer));

    Neuron ** neurons = malloc(sizeof (Neuron) * size);
    for(short neuron_index = 0; neuron_index < size ; neuron_index++)
        neurons[neuron_index] = create_neuron(prev_size);

    layer -> size = size;
    layer -> neurons = neurons;

    return layer;
}

typedef struct Network{
    short num_layers;
    Layer ** layers; // layers length is num_layers
}Network;

Network * create_network(const short sizes[], short num_layers){
    Network * network = (Network *) malloc(sizeof(Network));

    Layer ** layers = malloc(sizeof (Layer) * num_layers);
    short prev = 0;
    for(short layer_index = 0 ; layer_index < num_layers; layer_index++){
        layers[layer_index] = create_layer(sizes[layer_index], prev);
        prev = sizes[layer_index];
    }

    network -> num_layers = num_layers;
    network -> layers = layers;

    return network;
}

float sigmoid(float z){
    return 1.0f / (1.0f + expf(-z));
}

void print_network(Network * network){
    short num_layer = network -> num_layers;
    printf("Network < (length : %hd)", num_layer);
    for (short layer_index = 0; layer_index < num_layer; layer_index++){
        Layer * layer = network -> layers[layer_index];
        short size = layer -> size;
        printf("\n    Layer [ (size : %hd)", size);
        for (short neuron_index = 0; neuron_index < size; neuron_index++){
            Neuron * neuron = layer -> neurons[neuron_index];
            short num_weight = neuron -> num_weight;
            printf("\n        Neuron { (bias : %f - links : %hd)\n                "
                   "(\n                    %f", neuron -> bias, num_weight, neuron -> weights[0]);
            for (short weight_index = 1; weight_index < num_weight; weight_index++)
                printf(",\n                    %f", neuron -> weights[weight_index]);
            printf("\n                )\n        }");
        }
        printf("\n    ]");
    }
    printf("\n>\n");
}


float * feed_forward(Network * network, float inputs[]){

    float * prev_output;
    float * next_output = inputs;

    short num_layer = network -> num_layers;
    for (short layer_index = 1; layer_index < num_layer; layer_index++){
        // We compute the output of the layers (but not the first layer)

        Layer * layer = network -> layers[layer_index];
        Neuron ** neurons = layer -> neurons;
        short num_neurons = layer -> size;

        prev_output = next_output;  // next become prev,and we create a new next
        next_output = malloc(sizeof (float) * num_neurons);

        for (short neuron_index = 0; neuron_index < num_neurons; neuron_index++){
            Neuron * neuron = neurons[neuron_index];
            float * weights = neuron -> weights;

            float sum = 0;
            short num_weight = neuron -> num_weight;
            for (short weight_index = 0; weight_index < num_weight; weight_index++)
                sum += weights[weight_index] * prev_output[weight_index];

            next_output[neuron_index] = sigmoid(neuron -> bias + sum);
        }

    }

    return next_output;
}

