#include <stdlib.h>
#include <stdio.h>

void print_sudoku_1(int *grid)
{
    printf(" -");
    for (int i = 0; i < 21; i++)
    {
        printf("-");
    }
    printf(" \n");
    for (int i = 0; i < 9; i++)
    {
        printf("| ");
        for (int j = 0; j < 9; j++)
        {
            printf("%i ", grid[(i * 9) + j]);
            if ((j + 1) % 3 == 0 && j != 0)
            {
                printf("| ");
            }
        }
        printf(" \n");
        if ((i + 1) % 3 == 0 && i != 0)
        {
            printf(" ");
            for (int i = 0; i < 23; i++)
            {
                printf("-");
            }
            printf(" \n");
        }
    }
    printf(" \n");
}

int *parse_file(char *file_name)
{
    int *matrix = malloc(sizeof(int) * 81);
    for (size_t i = 0; i < 81; i++)
    {
        matrix[i] = 0;
    }
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL)
    {
        printf("Error: could not open file %s", file_name);
        exit(1);
    }
    char ch;
    size_t i = 0;
    while ((ch = fgetc(fp)) != EOF)
    {
        if (ch != ' ' && ch != 10)
        {
            if (ch == '.')
            {
                i++;
                continue;
            }
            matrix[i] = (ch - '0');
            i++;
        }
    }
    // close the file
    fclose(fp);
    return matrix;
}

void write_to_file(char *file_name, int *grid)
{
    FILE *fp;
    fp = fopen(file_name, "w");
    if (fp == NULL)
    {
        /* File not created hence exit */
        printf("Unable to create file.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 9; i++)
    {
        int k = 0;
        char *line = malloc(sizeof(char) * 13);
        for (int j = 0; j < 9; j++)
        {
            if (j % 3 == 0 && j != 0)
            {
                char c = ' ';
                line[k] = c;
                k++;
            }
            char c = grid[(i * 9) + j] + '0';

            line[k] = c;
            k++;
        }
        line[11] = '\n';
        line[12] = 0;
        fputs(line, fp);
        free(line);
        if ((i+1) % 3 == 0 && i != 0 && i !=8)
        {
            fputc('\n',fp);
        }
    }

    fclose(fp);
}

int possible_1(int y, int x, int *grid, int n)
{
    //column check
    for (int i = 0; i < 81; i += 9)
    {
        if (grid[x + i] == n)
        {
            return 0;
        }
    }
    //row check
    for (int j = 0; j < 9; j++)
    {
        if (grid[j + (y * 9)] == n)
        {
            return 0;
        }
    }
    int x_new = (x / 3) * 3;
    int y_new = (y / 3) * 3;
    int start_index = (x_new + y_new * 9);
    for (int i = 0; i <= 18; i += 9)
    {
        for (int j = 0; j < 3; j++)
        {
            if (grid[start_index + j + i] == n)
            {
                return 0;
            }
        }
    }
    return 1;
}

void solve_1(int *grid, int *coppy, char *solved)
{
    for (int y = 0; y < 9; y++)
    {
        for (int x = 0; x < 9; x++)
        {
            if (grid[x + y * 9] == 0)
            {
                for (int n = 1; n < 10; n++)
                {
                    if (possible_1(y, x, grid, n))
                    {
                        grid[x + y * 9] = n;
                        solve_1(grid, coppy,solved);
                        grid[x + y * 9] = 0;
                    }
                }
                return;
            }
        }
    }
    for (size_t i = 0; i < 81; i++)
    {
        coppy[i] = grid[i];
    }
    write_to_file(solved, coppy);
    return;
}
