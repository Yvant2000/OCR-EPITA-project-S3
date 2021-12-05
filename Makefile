# Makefile

CC = gcc -fsanitize=address -lm -D_DEFAULT_SOURCE -ldl -lSDL2 -lSDL2_image  `sdl2-config --cflags --libs` `pkg-config --cflags SDL2_image` `pkg-config --libs sdl SDL2_image` `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` -lSDL2_image
CPPFLAGS = -MMD 
CFLAGS = -Wall -Wextra -std=c99 -O3 -g 
LDFLAGS = 
LDLIBS = 
SRC = OCR.c main.c Solver/sudoku_res.c Image/image_maker.c Image/array_sort.c Image/basic_pixel.c Image/kernel.c Image/primary_filters.c  Image/otsu_binary.c  Image/rotation.c Image/sobel.c  Image/hough_functions.c Image/corners.c Image/decompose_image.c NeuralNetwork/Network.c NeuralNetwork/NetworkTraining.c NeuralNetwork/ImageTraining.c NeuralNetwork/Image.c GUI/interface.c

OBJ = ${SRC:.c=.o}
DEP = ${OBJ:.o=.d}

all: main

main: ${OBJ}
	$(CC) OCR.c main.c -c
	$(CC) -o main ${OBJ}

clean:
	${RM} ${OBJ} ${DEP} main
	make -C ./Image clean
	make -C ./NeuralNetwork clean
	make -C ./GUI clean

-include ${DEPS}

# END
