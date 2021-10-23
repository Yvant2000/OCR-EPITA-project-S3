//
// Created by Yves-Antoine on 23/10/2021.
//

#ifndef OCR_NETWORKTRAINING_H
#define OCR_NETWORKTRAINING_H

float * feed_forward(Network * network, const float * input_data);
void train_neural_network(Network * network,
                          float ** input_data, float ** expected_output, size_t data_size,
                          size_t epochs, float eta);

#endif //OCR_NETWORKTRAINING_H
