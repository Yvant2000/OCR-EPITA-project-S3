//
// Created by Yves-Antoine on 29/09/2021.
//

#ifndef OCR_VECTOR_H
#define OCR_VECTOR_H

typedef struct Vector{
    short key;
    struct Vector * next;
}Vector;

Vector * vector (const short values[], short size_of_array);

void print_vector(Vector * vector);

#endif //OCR_VECTOR_H
