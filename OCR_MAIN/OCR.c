//
// Created by yvesantoine on 04/12/2021.
//

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>
#include <string.h>

void solve_image(const char * path)
{
    char real_path[PATH_MAX];
    realpath(path, real_path);
    printf("%s\n", real_path);
    char * dir_name = dirname(real_path);
    printf("%s\n", dir_name);

    char new_path[PATH_MAX];
    strcpy(new_path, dir_name);
    strcat(new_path, "/numbers");
    printf("%s\n", new_path);

    return;
}
