//
// Created by Yves-Antoine on 23/10/2021.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Neuron{
    double bias;
    double delta_bias;

    double z;
    double delta_z;

    double activation;
    double delta_activation;

    double * weights;
    double * delta_weights;
} Neuron;

static inline double Random()
{
    return ((double)rand()) / ((double)RAND_MAX / 2.) - 1.; // NOLINT(cert-msc50-cpp)
}

Neuron * create_neuron(size_t num_weights, size_t num_out_weights){
    Neuron * neuron = (Neuron *) malloc(sizeof (Neuron));

    double * weights = malloc(sizeof (double) * num_weights);
    for(size_t weight_index = 0; weight_index < num_weights; weight_index++)
        weights[weight_index] = Random();

    neuron -> bias = Random();
    neuron -> delta_bias = 0.f;

    neuron -> z = 0.;
    neuron -> delta_z = 0.;

    neuron -> activation = 0.;
    neuron -> delta_activation = 0.;

    neuron -> weights = weights;
    neuron -> delta_weights = (double*) calloc(num_out_weights,  sizeof(double));

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

void save_network(Network * network, const char * file_name)
{
    /*
    ** Saves the given Network to a file.
    ** Will only save the bias and weights of the neurons.
    ** Return 0 if file can't be opened, 1 otherwise.
     */

    FILE * file = fopen(file_name, "w");
    if (file == NULL) {
        printf("Can't write on file %s.\n", file_name);
        exit(1);
    }

    size_t num_weights = 0;

    fprintf(file, "DO NOT EDIT THIS FILE, you may break it.\n");
    fprintf(file, "%zu\n", network -> num_layers);
    for (size_t layer_index = 0; layer_index < network -> num_layers; layer_index++){
        Layer * layer = network -> layers[layer_index];
        fprintf(file, "%zu\n", layer -> size);
        for (size_t neuron_index = 0; neuron_index < layer -> size; neuron_index++){
            Neuron * neuron = layer -> neurons[neuron_index];
            fprintf(file, "%lf\n", neuron -> bias);
            for (size_t weight_index = 0; weight_index < num_weights; weight_index++)
                fprintf(file, "%lf\n", neuron -> weights[weight_index]);
        }
        num_weights = layer -> size;
    }
    fclose(file);
}

static size_t string_to_size_t(const char * string)
{
    size_t i;
    sscanf(string, "%zu", &i);
    return i;
}

static double string_to_double(const char * string)
{
    double d;
    sscanf(string, "%lf", &d);
    return d;
}

void load_network(Network ** new_network, const char * filename)
{
    Network * network = malloc(sizeof (Network));

    FILE * file = fopen(filename, "r");
    if (file == NULL){
        printf("\"%s\" can't be opened. Make sure the file exist.", filename);
        exit(1);
    }

    size_t layer_sizes[256] = {0};

    size_t num_weights = 0;
    char line[256];
    fgets(line, sizeof(line), file); // skip first line
    fgets(line, sizeof(line), file); // Get the amount of layers
    network -> num_layers = string_to_size_t(line);

    network -> layers = malloc(sizeof (Layer *) * network -> num_layers);
    for (size_t layer_index = 0; layer_index < network -> num_layers; layer_index++){
        Layer * layer = malloc(sizeof (Layer));
        network -> layers[layer_index] = layer;
        fgets(line, sizeof(line), file); // Get the size of the layer
        layer -> size = string_to_size_t(line);

        layer_sizes[layer_index] = layer -> size;

        layer -> neurons = malloc(sizeof (Neuron *) * layer -> size);
        for (size_t neuron_index = 0; neuron_index < layer -> size; neuron_index++){
            Neuron * neuron = malloc(sizeof (Neuron));
            layer -> neurons[neuron_index] = neuron;
            fgets(line, sizeof(line), file); // Get bias of the neuron
            neuron -> bias = string_to_double(line);

            neuron -> weights = malloc(sizeof (double) * num_weights);
            //neuron -> delta_weights = malloc(sizeof (double) * num_weights);
            for (size_t weight_index = 0;weight_index < num_weights; weight_index++) {
                fgets(line, sizeof(line), file); // Get weights of the neuron
                neuron -> weights[weight_index] = string_to_double(line);
            }
            neuron -> delta_bias = 0.;
            neuron -> z = 0.;
            neuron -> delta_z = 0.;
            neuron -> activation = 0.;
            neuron -> delta_activation = 0.;
        }
        num_weights = layer -> size;
    }
    fclose(file);

    for (size_t layer_index = 0; layer_index < network -> num_layers; layer_index++)
        for(size_t neuron_index = 0; neuron_index < network -> layers[layer_index] -> size; neuron_index++)
            network -> layers[layer_index] -> neurons[neuron_index] -> delta_weights
            = malloc(sizeof (double) * layer_sizes[layer_index + 1]);

    (*new_network) = network;
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
                printf("(bias : %lf - links : %zu)\n                <    %lf",
                       neuron -> bias, num_weight, neuron -> weights[0]);

                for (size_t weight_index = 1; weight_index < num_weight; weight_index++)
                    printf(",    %lf", neuron -> weights[weight_index]);

                printf("    >\n       ");
            } else
                printf("INPUT");
            printf(" }");
        } printf("\n    ]");
    } printf("\n)\n");
} // print_network
