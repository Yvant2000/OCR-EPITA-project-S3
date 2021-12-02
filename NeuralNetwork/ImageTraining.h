//
// Created by yvesantoine on 01/12/2021.
//

#ifndef OCR_EPITA_PROJECT_S3_IMAGETRAINING_H
#define OCR_EPITA_PROJECT_S3_IMAGETRAINING_H

void train_for_image(Network * network);
void test_network_image(Network * network);
void infinite_train(Network * network, const char * save_path);

#endif //OCR_EPITA_PROJECT_S3_IMAGETRAINING_H
