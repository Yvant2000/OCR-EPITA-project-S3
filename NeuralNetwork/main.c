//
// Created by Yves-Antoine on 29/09/2021.
//

#include "Network.h"
#include "GatesTrainingData.h"

int main(){

    #define NUM_LAYER 3
    size_t sizes[NUM_LAYER] = {2, 3, 1};
    Network * network = create_network(sizes, NUM_LAYER);

    train_for_xor(network);

    print_network(network);

    test_inputs(network);

    return 0;
}
