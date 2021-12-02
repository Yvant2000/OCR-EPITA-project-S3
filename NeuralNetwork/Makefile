# Makefile

CC = gcc -fsanitize=address -lm -D_DEFAULT_SOURCE
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -std=c99 -O0 -g
LDFLAGS = `sdl2-config --cflags --libs` -lSDL2_image
LDLIBS =

OBJ = Network.o NetworkTraining.o ImageTraining.o main.o Image.o GatesTrainingData.o
DEP = ${OBJ:.o=.d}

all:main

main: ${OBJ}

clean:
	${RM} ${OBJ} ${DEP} main

-include ${DEP}

# END
