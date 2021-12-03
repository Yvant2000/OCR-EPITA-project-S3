CC=gcc 

CPPFLAGS= -MMD `pkg-config --cflags SDL2_image` 
CFLAGS= -Wall -Wextra -std=c99 -g -fsanitize=address -O3 -lm -ldl -lSDL2 -lSDL2_image -D__NO_INLINE__
LDFLAGS= 
LDLIBS= `pkg-config --libs sdl SDL2_image`
SCR = sudoku_res.c image_maker.c array_sort.c basic_pixel.c kernel.c primary_filters.c median_filters.c otsu_binary.c detect_trace.c detect_lines.c rotation.c sobel.c  hough_functions.c corners.c main.c 
OBJ = $(SCR:%.c = %.o)
all: main

main: $(OBJ)
	gcc  $(CFLAGS) -o $@ $^

clean:
	rm main
