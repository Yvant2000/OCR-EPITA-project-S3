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
}Neuron;

Neuron * create_neuron(size_t num_weights){
    Neuron * neuron = (Neuron *) malloc(sizeof (Neuron));

    float * weights = malloc(sizeof (float) * num_weights);
    for(size_t weight_index = 0; weight_index < num_weights; weight_index++){
        weights[weight_index] = 0.5f;
    }

    neuron -> bias = 0.5f;
    neuron -> num_weight = num_weights;
    neuron -> weights = weights;

    return neuron;
}


typedef struct Layer{
    size_t size;
    Neuron ** neurons; // neurons length is size
}Layer;

Layer * create_layer(size_t size, size_t prev_size){
    Layer * layer = (Layer *) malloc(sizeof (Layer));

    Neuron ** neurons = malloc(sizeof (Neuron) * size);
    for(size_t neuron_index = 0; neuron_index < size ; neuron_index++)
        neurons[neuron_index] = create_neuron(prev_size);

    layer -> size = size;
    layer -> neurons = neurons;

    return layer;
}

typedef struct Network{
    size_t num_layers;
    Layer ** layers; // layers length is num_layers
}Network;

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
}

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
}


float * feed_forward(Network * network, float inputs[]){

    float * prev_output;
    float * next_output = inputs;

    size_t num_layer = network -> num_layers;
    for (size_t layer_index = 1; layer_index < num_layer; layer_index++){
        // We compute the output of the layers (but not the first layer)

        Layer * layer = network -> layers[layer_index];
        Neuron ** neurons = layer -> neurons;
        size_t num_neurons = layer -> size;

        prev_output = next_output;  // next become prev,and we create a new next
        next_output = malloc(sizeof (float) * num_neurons);

        for (size_t neuron_index = 0; neuron_index < num_neurons; neuron_index++){
            Neuron * neuron = neurons[neuron_index];
            float * weights = neuron -> weights;

            float sum = 0;
            size_t num_weight = neuron -> num_weight;
            for (size_t weight_index = 0; weight_index < num_weight; weight_index++)
                sum += weights[weight_index] * prev_output[weight_index];

            next_output[neuron_index] = sigmoid(neuron -> bias + sum);
        }

    }

    return next_output;
}
/*
     def SGD(self, training_data, epochs, mini_batch_size, eta,
            test_data=None):
        """
         training_data en une liste de couple de listes de float. Oui, en résumé, c'est de la merde
         epochs est un int, tout simplement. C'est le nombre de fois qu'on recommence l'entrainement avec notre data
         mini_batch_size un int, qui correspond à la taille d'une petite découpe de training_data
         eta un float, correspond à la vitesse d'apprentissage (3 par exemple)
         """

        if test_data:
            n_test = len(test_data) # On a besoin de la taille de test_data (si utilisé)
        n = len(training_data) # on besoin de la taille de training_data

        for j in xrange(epochs):

            random.shuffle(training_data)  # On mélange aléatoirement training_data

            mini_batches = [
                training_data[k:k+mini_batch_size]
                for k in xrange(0, n, mini_batch_size)]  # En gros, on découpe training_data en petites parties de la taille de mini_batch_size


            for mini_batch in mini_batches:  # On update chaques petite partie de la data parties
                self.update_mini_batch(mini_batch, eta)  # On note qu'il y a moyen d'opti en façon cette action en même temps que le découpage de mini_batches

            if test_data:  # Si on a de quoi tester,
                print "Epoch {0}: {1} / {2}".format(
                    j, self.evaluate(test_data), n_test)  # On affiche les résultats des tests fait pendant l'entrainement
            else:
                print "Epoch {0} complete".format(j)  # Sinon on affiche ou on en est
 */

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
}

void compute_delta(float * input_data, float * expected_output, float * delta_bias, float * delta_weights, size_t num_weight){
}

void update_mini_batch(Network * network,
                       float ** input_data, float ** expected_output, size_t training_data_size,
                       size_t mini_batch_index, size_t mini_batch_size,
                       float eta){

    size_t num_layers = network -> num_layers;
    for(size_t layer_index = 0; layer_index < num_layers; layer_index++){
        Layer * layer = network -> layers[layer_index];

        size_t layer_size = layer -> size;
        for(size_t neuron_index = 0; neuron_index < layer_size; neuron_index++){
            Neuron * neuron = layer -> neurons[neuron_index];
            size_t num_weight = neuron -> num_weight;

            float nabla_bias = 0.f;   // init nabla bias to 0
            float * nabla_weights = calloc(num_weight, sizeof(float));  // init nabla weights to 0

            size_t mini_batch_max = mini_batch_index + mini_batch_size;
            for(size_t data_index = mini_batch_index; (data_index < mini_batch_max) && (data_index < training_data_size); data_index++){
                float delta_bias = 0.f;
                float * delta_weights = calloc(num_weight, sizeof(float));

                compute_delta(input_data[data_index], expected_output[data_index], &delta_bias, delta_weights, num_weight); // we compute the value of deltas

                nabla_bias += delta_bias; // add delta to nabla
                for (size_t weight_index = 0; weight_index < num_weight; weight_index++)
                    nabla_weights[weight_index] += delta_weights[weight_index]; // add delta to nabla

                free(delta_weights);
            }

            neuron -> bias -= (eta / (float)mini_batch_size) * nabla_bias;  // update neuron's bias to nabla_bias
            for (size_t weight_index = 0; weight_index < num_weight; weight_index++)  // update neuron's weights to nabla_weights
                neuron -> weights[weight_index] -= (eta / (float)mini_batch_size) * nabla_weights[weight_index];

            free(nabla_weights);
        }
    }


}

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
        printf("Epoch %hu / %hu\n", step, epochs);
    }
}

