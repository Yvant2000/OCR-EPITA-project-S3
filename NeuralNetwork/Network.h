//
// Created by Yves-Antoine on 29/09/2021.
//

#ifndef OCR_EPITA_PROJECT_S3_NETWORK_H
#define OCR_EPITA_PROJECT_S3_NETWORK_H

#include <stddef.h>

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

typedef struct Layer{
    size_t size;
    Neuron ** neurons;
}Layer;

typedef struct Network{
    size_t num_layers;
    Layer ** layers;
}Network;

Network * create_network(const size_t sizes[], size_t num_layers);
void print_network(Network* network);
void delete_network(Network * network);
void save_network(Network * network, const char * file_name);
void load_network(Network ** network, const char * filename);

#endif //OCR_EPITA_PROJECT_S3_NETWORK_H