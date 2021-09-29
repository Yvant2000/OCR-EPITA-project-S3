//
// Created by Yves-Antoine on 29/09/2021.
//

#include <stdlib.h>
#include <stdio.h>

typedef struct Vector{
    short key;
    struct Vector * next;
}Vector;

Vector * vector (const short values[], size_t size_of_array){

    Vector * vector1 = (struct Vector *) malloc(sizeof (Vector));
    vector1 -> key = values[0];

    Vector * tmp = vector1;

    for (int i = 1; i < size_of_array; i++){
        Vector * next = (Vector *) malloc(sizeof (Vector));
        next -> key = values[i];
        next -> next = NULL;
        tmp -> next = next;
        tmp = next;
    }
    return vector1;
}

void print_vector(Vector * vector){
    printf("vector < %hd", vector -> key);
    while(vector -> next != NULL){
        vector = vector -> next;
        printf(" -> %hd", vector->key);
    }
    printf(" >");
}