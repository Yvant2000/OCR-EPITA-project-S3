//
// Created by Yves-Antoine on 29/09/2021.
//

#ifndef OCR_EPITA_PROJECT_S3_NETWORK_H
    #define OCR_EPITA_PROJECT_S3_NETWORK_H

    typedef struct Neuron Neuron;

    typedef struct Layer{
        size_t size;
        Neuron ** neurons;
    }Layer;

    typedef struct Network{
        size_t num_layers;
        Layer ** layers;
    }Network;

    Network * create_network(const size_t sizes[], size_t num_layers);
    float * feed_forward(Network * network, float inputs[]);
    void print_network(Network * network);
    void train_network(
            Network * network,
            float ** input_data, float ** expected_output, size_t training_data_size,
            size_t epochs, size_t mini_batch_size, float eta);

#endif //OCR_EPITA_PROJECT_S3_NETWORK_H
