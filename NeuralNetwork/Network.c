//
// Created by Yves-Antoine on 29/09/2021.
//

#include <stdlib.h>
#include <math.h>
#include <stdio.h>


typedef struct Neuron{
    float bias;
    size_t num_weight;
    float * weights; // weights length is num_weight;
} Neuron;

Neuron * create_neuron(size_t num_weights){
    Neuron * neuron = (Neuron *) malloc(sizeof (Neuron));

    float * weights = malloc(sizeof (float) * num_weights);
    for(size_t weight_index = 0; weight_index < num_weights; weight_index++){
        weights[weight_index] = ((float)(rand() % 10) )/ 10.f;  // NOLINT(cert-msc50-cpp)
    }

    neuron -> bias = ((float)(rand() % 10) )/ 10.f;  // NOLINT(cert-msc50-cpp)
    neuron -> num_weight = num_weights;
    neuron -> weights = weights;

    return neuron;
} // create_neuron


typedef struct Layer{
    size_t size;
    Neuron ** neurons; // neurons length is size
} Layer;

Layer * create_layer(size_t size, size_t prev_size){
    Layer * layer = (Layer *) malloc(sizeof (Layer));

    Neuron ** neurons = malloc(sizeof (Neuron) * size);
    for(size_t neuron_index = 0; neuron_index < size ; neuron_index++)
        neurons[neuron_index] = create_neuron(prev_size);

    layer -> size = size;
    layer -> neurons = neurons;

    return layer;
} // create_layer

typedef struct Network{
    size_t num_layers;
    Layer ** layers; // layers length is num_layers
} Network;

Network * create_network(const size_t sizes[], size_t num_layers){
    Network * network = (Network *) malloc(sizeof(Network));

    Layer ** layers = malloc(sizeof (Layer) * num_layers);
    size_t prev = 0;
    for(size_t layer_index = 0 ; layer_index < num_layers; layer_index++){
        layers[layer_index] = create_layer(sizes[layer_index], prev);
        prev = sizes[layer_index];
    }

    network -> num_layers = num_layers;
    network -> layers = layers;

    return network;
} // create_network

float sigmoid(float z){
    return 1.f / (1.f + expf(-z));
}

float sigmoid_prime(float z){
    float exp_minus_z = expf(-z);
    return 1.f / ( (1.f/exp_minus_z) + 2.f + exp_minus_z); // Same result than "sigmoid(z)*(1.0f-sigmoid(z))" but faster
}




void print_network(Network * network){
    size_t num_layer = network -> num_layers;
    printf("Network ( (length : %zu)", num_layer);

    for (size_t layer_index = 0; layer_index < num_layer; layer_index++){
        Layer * layer = network -> layers[layer_index];
        size_t size = layer -> size;
        printf("\n    Layer [ (size : %zu)", size);

        for (size_t neuron_index = 0; neuron_index < size; neuron_index++){
            Neuron * neuron = layer -> neurons[neuron_index];
            size_t num_weight = neuron -> num_weight;

            printf("\n        Neuron { ");
            if (num_weight){
                printf("(bias : %f - links : %zu)\n                <\n                    %f",
                       neuron -> bias, num_weight, neuron -> weights[0]
                       );

                for (size_t weight_index = 1; weight_index < num_weight; weight_index++)
                    printf(",\n                    %f", neuron -> weights[weight_index]);

                printf("\n                >\n       ");
            }
            else
                printf("INPUT");
            printf(" }");
        }
        printf("\n    ]");
    }
    printf("\n)\n");
} // print_network


float * feed_forward__(Network * network, float inputs[], float ** zs){

    float * next_output = inputs;

    size_t num_layer = network -> num_layers;
    for (size_t layer_index = 1; layer_index < num_layer; layer_index++){
        // We compute the output of the layers (but not the first layer)

        Layer * layer = network -> layers[layer_index];
        Neuron ** neurons = layer -> neurons;
        size_t num_neurons = layer -> size;

        float * prev_output = next_output;  // next become prev,and we create a new next
        next_output = malloc(sizeof (float) * num_neurons);

        if (zs != NULL) // If needed, we get zs
            zs[layer_index] = malloc(sizeof (float *) * num_neurons);


        for (size_t neuron_index = 0; neuron_index < num_neurons; neuron_index++){
            Neuron * neuron = neurons[neuron_index];
            float * weights = neuron -> weights;


            float sum = 0;
            size_t num_weight = neuron -> num_weight;
            for (size_t weight_index = 0; weight_index < num_weight; weight_index++)
                sum += weights[weight_index] * prev_output[weight_index];

            float z = neuron -> bias + sum;

            next_output[neuron_index] = sigmoid(z);

            if (zs != NULL) // If needed, we get zs (last use of z)
                zs[layer_index][neuron_index] = z;
        }
        free(prev_output);
    }
    return next_output;
}  // feed_forward__

float * feed_forward(Network * network, float inputs[]) {
    return feed_forward__(network, inputs, NULL);
} // feed_forward


void shuffle_data(float ** input_data, float ** expected_output, size_t training_data_size){
    for(size_t data_index = 0; data_index < training_data_size; data_index++){
        size_t random_index = rand() % training_data_size; // NOLINT(cert-msc50-cpp) - don't care if function is predictable

        float * temp = input_data[data_index];
        input_data[data_index] = input_data[random_index];
        input_data[random_index] = temp;

        temp = expected_output[data_index];
        expected_output[data_index] = expected_output[random_index];
        expected_output[random_index] = temp;
    }
} // shuffle_data (in the mini-batch)

void update_mini_batch(Network * network,
                       float ** input_data, float ** expected_output, size_t training_data_size,
                       size_t mini_batch_index, size_t mini_batch_size,
                       float eta){

    float learning_speed = eta / ((float) mini_batch_size); // The learning is `eta / len(mini_batch)`


    size_t num_layers = network -> num_layers; // amount of layers in the network

    size_t mini_batch_max = mini_batch_index + mini_batch_size;  // maximum index of the mini_batch
    for(size_t data_index = mini_batch_index; (data_index < mini_batch_max) && (data_index < training_data_size); data_index++) { // for each mini batch

        float * final_expected_output = expected_output[data_index]; // data in output of the mini_batch (y)


        /* Our goal is that f(x) = y, where f is our neural network, x is the input image and y is the right output */


        ///////// DELTA IS `(feedforward_Out - y) * sigmoid_prime(last z)` /////////

        // Compute DELTA between output and expected output
        size_t output_layer_size = network -> layers[network -> num_layers - 1] -> size;

        float ** zs = malloc(sizeof (float *) * num_layers); // zs is the value to put into sigmoid
        float * delta = feed_forward__(network, input_data[data_index], zs); // delta = (output - expected_output) * sigmoid_prime(zs[-1])
        for (size_t output_index = 0; output_index < output_layer_size; output_index++){
            delta[output_index] -= final_expected_output[output_index];
            delta[output_index] *= sigmoid_prime(zs[num_layers - 1][output_index]);
        }

        ///////////////////////////////////////////////////////////////////////////

        for (size_t layer_index = num_layers - 1; layer_index > 0; layer_index--) { // for each layer but the last
            Layer * layer = network -> layers[layer_index];  // current layer
            size_t layer_size = layer -> size;  // size of the current layer


            float * z = zs[layer_index]; // current z value in backdrop
            // sp = sigmoid_prime(z);

            float * old_delta = delta;
            delta = malloc(sizeof (float) * network -> layers[layer_index - 1] -> size); // We compute delta for the next iteration

            for (size_t neuron_index = 0; neuron_index < layer_size; neuron_index++) { // for each neuron
                Neuron * neuron = layer -> neurons[neuron_index]; // current neuron
                size_t num_weight = neuron -> num_weight; // amount of connections for the neuron
                float * weights = neuron -> weights;  // connections of the neuron

                float sp = sigmoid_prime(z[neuron_index]);

                delta[neuron_index] = 0;

                neuron -> bias -= learning_speed * old_delta[neuron_index];
                for (size_t weight_index = 0; weight_index < num_weight; weight_index++){

                    delta[neuron_index] += old_delta[neuron_index] * weights[weight_index] * sp; // update delta

                    weights[weight_index] -= learning_speed * old_delta[neuron_index] * sigmoid(zs[layer_index][weight_index]);

                }

            }

            free(old_delta);
        }

        for (size_t layer_index = 0; layer_index < num_layers; layer_index++)
            free(zs[layer_index]);
        free(zs);

        free(delta);

    }
} // update_mini_batch


void train_network(Network * network,
                   float ** input_data, float ** expected_output, size_t training_data_size,
                   unsigned short epochs, size_t mini_batch_size, float eta){
    /*
     * Train the neural network using mini-batch stochastic gradient descent.
     */

    for (unsigned short step = 1; step <= epochs; step++){
        shuffle_data(input_data, expected_output, training_data_size);

        for (size_t mini_batch_index = 0; mini_batch_index < training_data_size; mini_batch_index += mini_batch_size)
            update_mini_batch(network,
                              input_data,expected_output, training_data_size,
                              mini_batch_index, mini_batch_size,
                              eta);

        printf("Epoch %hu / %hu\n", step, epochs); // printf is slow, we could let people chose to display it or not

        // TODO show advancements in the training to prove the improvements
    }
} // train network
