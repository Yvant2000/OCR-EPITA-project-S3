//
// Created by Yves-Antoine on 23/10/2021.
//

#ifndef OCR_GATESTRAININGDATA_H
#define OCR_GATESTRAININGDATA_H


void train_for_xor(Network * network);
void train_for_or(Network * network);
void train_for_and(Network * network);
void train_for_nand(Network * network);

void test_inputs(Network * network);

#endif //OCR_GATESTRAININGDATA_H
