//
// Created by Yves-Antoine on 29/09/2021.
//

#ifndef OCR_EPITA_PROJECT_S3_NETWORK_H
#define OCR_EPITA_PROJECT_S3_NETWORK_H

#include <stddef.h>

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

typedef struct Layer{
    size_t size;
    Neuron ** neurons;
}Layer;

typedef struct Network_old{
    size_t num_layers;
    Layer ** layers;
}Network;

Network * create_network(const size_t sizes[], size_t num_layers);
void print_network(Network* network);
void delete_network(Network * network);

#endif //OCR_EPITA_PROJECT_S3_NETWORK_H