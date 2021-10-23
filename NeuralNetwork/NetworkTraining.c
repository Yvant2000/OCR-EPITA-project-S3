//
// Created by Yves-Antoine on 23/10/2021.
//
#include <malloc.h>
#include "Network.h"
#include "math.h"

void update_weights(Network * network, float eta)
{
    size_t num_layers = network -> num_layers;
    for(size_t layer_index = 0; layer_index < num_layers - 1; layer_index++){  // foreach layer in the network
        Layer * layer = network -> layers[layer_index];
        Layer * next_layer = network -> layers[layer_index + 1];

        size_t num_neurons = layer -> size;
        for(size_t neuron_index = 0; neuron_index < num_neurons; neuron_index++){ // foreach neuron in the layer
            Neuron * neuron = layer -> neurons[neuron_index];

            for(size_t weight_index = 0; weight_index < next_layer -> size; weight_index++){ // foreach weight in the neuron
                Neuron * out_neuron = next_layer -> neurons[weight_index];
                out_neuron -> weights[neuron_index] -= (eta * neuron -> delta_weights[weight_index]); // set the new value of the weight
            }
            neuron -> bias -= (eta * neuron -> delta_bias); // set the new value of the bias
        }
    }
}

void backdrop(Network * network, const float * expected_output)
{
    // Output Layer

    Layer * output_layer = network -> layers[network -> num_layers - 1];
    Layer * prev_output_layer = network -> layers[network -> num_layers - 2];
    size_t num_neurons_output_layer = output_layer -> size;
    size_t num_neuron_weights = prev_output_layer -> size;

    for(size_t neuron_index = 0; neuron_index < num_neurons_output_layer; neuron_index++){  // foreach neuron in the output layer
        Neuron * neuron = output_layer -> neurons[neuron_index];
        neuron -> delta_z = (neuron -> activation - expected_output[neuron_index]) * (neuron -> activation) * (1 - neuron -> activation);

        for(size_t weight_index = 0; weight_index < num_neuron_weights; weight_index++){ // foreach weight of the neuron
            Neuron * prev_layer_neuron = prev_output_layer -> neurons[weight_index];
            prev_layer_neuron -> delta_weights[neuron_index] = (neuron -> delta_z * prev_layer_neuron -> activation); // compute the difference between the
            prev_layer_neuron -> delta_activation = neuron -> weights[weight_index] * neuron -> delta_z;              // expected output and the actual output
        }

        neuron -> delta_bias = neuron -> delta_z;
    }

    // Hidden Layers
    size_t num_layers = network -> num_layers;
    for(size_t layer_index = num_layers - 2; layer_index > 0; --layer_index){ // foreach layer in the network
        Layer * layer = network -> layers[layer_index];
        Layer * prev_layer = network -> layers[layer_index - 1];
        size_t num_neurons = layer -> size;
        size_t num_weights = prev_layer -> size;

        for(size_t neuron_index = 0; neuron_index < num_neurons; neuron_index++){ // foreach neuron in the neuron
            Neuron * neuron = layer -> neurons[neuron_index];

            if(neuron -> z >= 0)
                neuron -> delta_z = neuron -> delta_activation;
            else
                neuron -> delta_z = 0.f;

            for(size_t weight_index = 0; weight_index < num_weights; weight_index++){ // foreach weight in the neuron
                Neuron * prev_layer_neuron = prev_layer -> neurons[weight_index];
                prev_layer_neuron -> delta_weights[neuron_index] = neuron -> delta_z * prev_layer_neuron -> activation; // update the delta for the next neurons

                if(layer_index > 1)
                    prev_layer -> neurons[weight_index] -> delta_activation = neuron -> weights[weight_index] * neuron -> delta_z;
            }

            neuron -> delta_bias = neuron -> delta_z;
        }
    }
}

float sigmoid(float z){
    return 1.f / (1.f + expf(-z));
}

/*
float sigmoid_prime(float z){
    float exp_minus_z = expf(-z);
    return 1.f / ( (1.f/exp_minus_z) + 2.f + exp_minus_z); // Same result than "sigmoid(z)*(1.0f-sigmoid(z))" but faster
}
 */


void feed_forward__(Network * network){
/* Compute the values of the network*/
    size_t num_layers = network -> num_layers;
    for(size_t layer_index = 1; layer_index < num_layers; layer_index++){
        Layer * layer = network -> layers[layer_index];
        Layer * prev_layer = network -> layers[layer_index - 1];
        size_t num_neurons = layer -> size;
        size_t num_neurons_prev_layer = prev_layer -> size;

        for(size_t neuron_index = 0; neuron_index < num_neurons; neuron_index++){
            Neuron * neuron = layer -> neurons[neuron_index];
            neuron -> z = neuron -> bias;

            for(size_t prev_neuron_index = 0; prev_neuron_index < num_neurons_prev_layer; prev_neuron_index++) {
                Neuron * prev_layer_neuron = prev_layer -> neurons[prev_neuron_index];
                neuron -> z  += ((neuron -> weights[prev_neuron_index]) * (prev_layer_neuron -> activation));
            }

            // Relu Activation Function for Hidden Layers
            if(layer_index < num_layers - 1)
                if((neuron -> z) < 0)
                    neuron -> activation = 0;
                else
                    neuron -> activation = neuron -> z;

                // Sigmoid Activation function for Output Layer
            else
                neuron -> activation = sigmoid(neuron -> z);
        }
    }
}

void feed_input(Neuron ** neurons, size_t input_layer_size, const float * input){
    /* Feed the input layer with the given input */
    for(size_t neuron_index = 0; neuron_index < input_layer_size; neuron_index++) // Just set the value of each neuron to the input
        neurons[neuron_index] -> activation = input[neuron_index];
}

float * feed_forward(Network * network, const float * input_data){
    /*
    ** Return the output of the neural network for a given input
     */

    feed_input(network -> layers[0] -> neurons, network -> layers[0] -> size, input_data); //  input the data
    feed_forward__(network);  // compute the result

    Layer * output_layer = network -> layers[network -> num_layers - 1];
    size_t layer_size = output_layer -> size;

    float * output = malloc(layer_size * sizeof (float));

    for (size_t neuron_index = 0; neuron_index < layer_size; neuron_index++) // for each output neuron
        output[neuron_index] = output_layer -> neurons[neuron_index] -> activation; // we get the output value

    return output;  // return the value given by the network
}

void shuffle_data(float ** input_data, float ** expected_output, size_t training_data_size){
    /*
    ** shuffle_data for input_data and expected_output.
    ** The data stay connected - example :  (abc) (a'b'c') -> (cba) (c'b'a')
     */
    for(size_t data_index = 0; data_index < training_data_size; data_index++){
        size_t random_index = rand() % training_data_size; // NOLINT(cert-msc50-cpp) - don't care if function is predictable

        {float * temp = input_data[data_index];
            input_data[data_index] = input_data[random_index];
            input_data[random_index] = temp;}
        {float * temp = expected_output[data_index];
            expected_output[data_index] = expected_output[random_index];
            expected_output[random_index] = temp;}
    }
} // shuffle_data

void train_neural_network(Network * network, float ** input_data, float ** expected_output, size_t data_size, size_t epochs, float eta)
/* Trains the Network with*/
{

    Neuron ** input_neurons = network -> layers[0] -> neurons;
    size_t input_size = network -> layers[0] -> size;

    // Gradient Descent
    for (size_t epoch = 0; epoch < epochs; epoch++) {  // for each iteration of the training
        // shuffle_data(input_data, expected_output, data_size);  // shuffle the data for better result on big data
        for (size_t data_index = 0; data_index < data_size; data_index++) {  // we assume mini_batch = 1
            feed_input(input_neurons, input_size, input_data[data_index]);  // input the data into the first layer
            feed_forward__(network);  // compute the output of the network for the given input
            backdrop(network, expected_output[data_index]);  // compute the error of the network
            update_weights(network, eta);  // update de weights of the network
        }
    }
}