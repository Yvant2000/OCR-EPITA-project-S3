//
// Created by Yves-Antoine on 29/09/2021.
//

# include <stdlib.h>
# include <math.h>



typedef struct Neuron{
    float biases;
    short num_weight;
    float * weights; // weights length is num_weight;
}Neuron;

Neuron * create_neuron(short num_weights){
    Neuron * neuron = (Neuron *) malloc(sizeof (Neuron));

    float weights[num_weights];
    for(short weight_index = 0; weight_index < num_weights; weight_index++){
        weights[weight_index] = 0.5f;
    }

    neuron -> biases = 0.5f;
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

    Neuron * neurons[size];
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
    struct Network * network = (Network *) malloc(sizeof(Network));

    Layer * layers[num_layers];
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


float * feed_forward(Network * network, float inputs[]){

    ///// We admit that the size of inputs is the size of the first layer
    float * output = malloc(sizeof (float)* network -> layers[0] -> size);

    float * prev = inputs;
    float * next;

    // short output_layer_size = network -> layers[network -> num_layers - 1] -> size;
    for (int layer_index = 0; layer_index < network -> num_layers; layer_index++){
        Layer * layer = network -> layers[layer_index];
        Neuron ** neurons = layer -> neurons;
        short num_neurons = layer -> size;

        //for ()

    }

    return output;
}

