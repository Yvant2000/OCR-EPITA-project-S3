//
// Created by yvesantoine on 04/12/2021.
//
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>
#include <string.h>

#include "Image/decompose_image.h"
#include "NeuralNetwork/Network.h"
#include "NeuralNetwork/ImageTraining.h"
#include "Solver/sudoku_res.h"

void solve_image(char * path, char * output)
{
    apply_solve(path);

    int sudoku[81];
    Network * network;
    load_network(&network, "./NeuralNetwork/Network.NN");

    for (int i = 1; i <= 81; i++){
        char real_path[PATH_MAX];
        realpath(path, real_path);
        char folder_path[PATH_MAX];
        strcpy(folder_path, dirname(real_path));
        strcat(folder_path, "/numbers/num");
        char number_str[4];
        sprintf(number_str, i < 10 ? "0%d" : "%d", i);
        strcat(folder_path, number_str);
        strcat(folder_path, ".bmp");

        int result = image_to_digit(network, folder_path);
        sudoku[i - 1] = result;
    }
    write_to_file(output, sudoku);
    int * matrix = parse_file(output);
    char solved_path[PATH_MAX];
    strcpy(solved_path, output);
    strcat(solved_path, "_solved");
    int solved[81];
    solve_1(matrix, solved, solved_path);
    return;
}

