//
// Created by Yves-Antoine on 29/09/2021.
//

#include "Network.h"
#include "Image.h"
#include "ImageTraining.h"
#include "GatesTrainingData.h"

int main(){

/////////////  XOR GATE TEST SUITE
//
//#define NUM_LAYER 3
//    size_t sizes[NUM_LAYER] = {2, 3, 1};
//    Network * network = create_network(sizes, NUM_LAYER);
//
//    train_for_xor(network);
//    test_inputs_gate(network);
//    exit(0);
//
////////////////////


////// HOW TO CREATE A NEW NETWORK
//
//    #define NUM_LAYER 3
//    size_t sizes[NUM_LAYER] = {784,
//                               15,
//                               10}; //image = 28*28 = 784
//    Network * network = create_network(sizes, NUM_LAYER);
//
////////////

// HOW TO LOAD A NETWORK  ///
//
    Network * network;
    load_network(&network, "Network.NN");
//
////////////

    //train_for_image(network);  // TRAIN A NETWORK
    //test_network_image(network);  // TEST A NETWORK
    //infinite_train(network, "Network.NN");   // TRAIN, TEST AND SAVE UNTIL USER FORCE QUITS

    //print_network(network); // SHOW NETWORK
    //save_network(network, "Network.NN");  // SAVE NETWORK


/////////// USAGE OF TRAINED NEURAL NETWORK
//
//    const char path[] = "./ImageDataBase/numbers/num15.bmp";
//    printf("%d \n", image_to_digit(network, path));
//    double * result = image_to_recognized_array(network, path);
//    for(char index = 0; index < 10; index++) printf("%lf ", result[index]);printf("\n");
//    free(result);
//
/////////////////

    delete_network(network);
    return 0;
}
