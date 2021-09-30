//
// Created by Yves-Antoine on 29/09/2021.
//

#include <stdio.h>
# include "Network.h"


int main(){

    short sizes[] = {2,3,1};
    Network * network = create_network(sizes, 3);

    // float inputs[] = {0.5f, 0.5f};
    // float * test_output = feed_forward(network, inputs);

    print_network(network);

    return 0;
}