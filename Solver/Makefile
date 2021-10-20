CC=gcc 

CPPFLAGS= -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3 -g -fsanitize=address
LDFLAGS= 
LDLIBS=
SCR = sudoku_res.c main.c
OBJ = $(SCR:%.c = %.o)
all: main

main: $(OBJ)
	gcc  $(CFLAGS) -o $@ $^