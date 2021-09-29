//
// Created by Yves-Antoine on 29/09/2021.
//
#include <stdlib.h>

struct Network{

};

struct Network * create_network(){
    struct Network * network = (struct Network *) malloc(sizeof(struct Network));
    return network;
}