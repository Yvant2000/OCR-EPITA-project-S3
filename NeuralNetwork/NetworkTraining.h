//
// Created by Yves-Antoine on 23/10/2021.
//

#ifndef OCR_NETWORKTRAINING_H
#define OCR_NETWORKTRAINING_H

double * feed_forward(Network * network, const double * input_data);
void train_neural_network(Network * network,
                          double ** input_data, double ** expected_output, size_t data_size,
                          size_t epochs, double eta);

#endif //OCR_NETWORKTRAINING_H
