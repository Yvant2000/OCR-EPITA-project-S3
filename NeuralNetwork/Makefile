# Makefile

CC = gcc -fsanitize=address -lm
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -std=c99 -O0 -g
LDFLAGS = `sdl2-config --cflags --libs` -lSDL2_image
LDLIBS =

OBJ = Network.o NetworkTraining.o GatesTrainingData.o main.o Image.o
DEP = ${OBJ:.o=.d}

all:main

main: ${OBJ}

clean:
	${RM} ${OBJ} ${DEP} main

-include ${DEP}

# END
