//
// Created by Yves-Antoine on 29/09/2021.
//

#include "Network.h"
#include "Image.h"
#include "ImageTraining.h"
#include "GatesTrainingData.h"

int main(){

//    #define NUM_LAYER 3
//    size_t sizes[NUM_LAYER] = {784, 15, 10}; //image = 28*28 = 784
//    Network * network = create_network(sizes, NUM_LAYER);

    Network * network;
    load_network(&network, "Network.NN");

///////////XOR GATE
//#define NUM_LAYER 3
//    size_t sizes[NUM_LAYER] = {2, 3, 1};
//    Network * network = create_network(sizes, NUM_LAYER);
//
//    train_for_xor(network);
//    test_inputs_gate(network);
//////////////////

    //train_for_image(network);
    //test_network_image(network);
    infinite_train(network, "Network.NN");

    //print_network(network);
    save_network(network, "Network.NN");
    delete_network(network);


    return 0;
}
