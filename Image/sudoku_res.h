#include <stdlib.h>
#include <stdio.h>


#ifndef SUDOKU_RES_H
#define SUDOKU_RES_H
void solve_1(int *grid, int *coppy, char *solved);
void print_sudoku_1(int *grid);
int possible_1(int y, int x, int *grid, int n);
int *parse_file(char *file_name);
void write_to_file(char *file_name, int *grid);
#endif