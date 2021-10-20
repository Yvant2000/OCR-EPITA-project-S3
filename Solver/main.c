#include <stdlib.h>
#include <stdio.h>
#include "sudoku_res.h"

int main(int argc, char **argv)
{
    if(argc<3){
        printf("Please provide the file to resolve followed by the directory where you wish to save the resolved file\n");
        return 1;
    }
    else{
        int *grid1 = malloc(sizeof(int)*81);
    int *matrix = malloc(sizeof(int) * 81);
    matrix = parse_file(argv[1]);
    solve_1(matrix,grid1,argv[2]);
    free(grid1);
    free(matrix);
    return 0;
    }
    
    
}