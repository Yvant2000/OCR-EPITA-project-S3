# Makefile

CC = gcc -fsanitize=address -lm -D_DEFAULT_SOURCE  -ldl -lSDL2 -lSDL2_image
CPPFLAGS= -MMD
CFLAGS= -Wall -Wextra -std=c99 -g -O3
LDFLAGS= `pkg-config --cflags SDL2_image` `pkg-config --libs sdl SDL2_image`
LDLIBS=
SCR = sudoku_res.c image_maker.c array_sort.c basic_pixel.c kernel.c primary_filters.c  otsu_binary.c  rotation.c sobel.c  hough_functions.c corners.c decompose_image.c main.c
OBJ = ${SCR:.c=.o}
DEP = ${OBJ:.o=.d}

all: main

main:${OBJ}

clean:
	${RM} ${OBJ} ${DEP} main

-include ${DEP}

# END