# Makefile

CC = gcc -fsanitize=address -lm
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -std=c99 -O0 -g
LDFLAGS =
LDLIBS =

OBJ = Network.o NetworkTraining.o GatesTrainingData.o main.o
DEP = ${OBJ:.o=.d}

all:main

main: ${OBJ}

clean:
	${RM} ${OBJ} ${DEP} main

-include ${DEP}

# END
