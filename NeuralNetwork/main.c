//
// Created by Yves-Antoine on 29/09/2021.
//

#include "Network.h"
#include "GatesTrainingData.h"
#include "Image.h"

int main(){

    #define NUM_LAYER 3
    size_t sizes[NUM_LAYER] = {784, 15, 10}; //image = 28*28 = 784
    Network * network = create_network(sizes, NUM_LAYER);

    // SDL_Surface * image = load_image("test.jpg");
    // float * array = image_to_array(image);

    //print_network(network);
    save_network(network, "Network.NN");
    delete_network(network);


    return 0;
}
