# Makefile

CC = gcc -fsanitize=address -lm -D_DEFAULT_SOURCE -ldl -lSDL2 -lSDL2_image
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -std=c99 -O0 -g
LDFLAGS = `sdl2-config --cflags --libs` `pkg-config --cflags SDL2_image` `pkg-config --libs sdl SDL2_image` -lSDL2_image
LDLIBS =

SRC = OCR.c main.c Image/sudoku_res.c Image/image_maker.c Image/array_sort.c Image/basic_pixel.c Image/kernel.c Image/primary_filters.c  Image/otsu_binary.c  Image/rotation.c Image/sobel.c  Image/hough_functions.c Image/corners.c Image/decompose_image.c

OBJ = ${SRC:.c=.o}
DEP = ${OBJ:.o=.d}

all: main

main: ${OBJ}
	$(CC) OCR.c main.c -c
	$(CC) -o main ${OBJ}

clean:
	${RM} ${OBJ} ${DEP} main
	make -C ./Image clean

-include ${DEPS}

# END
