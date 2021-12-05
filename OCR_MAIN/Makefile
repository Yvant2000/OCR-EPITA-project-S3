# Makefile


PROJDIR := $(realpath $(CURDIR)/..)

SDIRS = Image OCR_MAIN
TDIRS = OCR_MAIN
SOURCEDIRS = $(foreach dir, $(SDIRS), $(addprefix $(PROJDIR)/, $(dir)))

INCLUDES = $(foreach dir, $(SOURCEDIRS), $(addprefix -I, $(dir)))
VPATH = $(SOURCEDIRS)

SOURCES = $(foreach dir,$(SOURCEDIRS),$(wildcard $(dir)/*.c))
OBJS := $(subst $(SOURCEDIR),$(BUILDDIR),$(SOURCES:.c=.o))
DEPS = $(OBJS:.o=.d)

CC = gcc -fsanitize=address -lm -D_DEFAULT_SOURCE
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -std=c99 -O0 -g
LDFLAGS = `sdl2-config --cflags --libs` -lSDL2_image
LDLIBS =

SRC = OCR.c main.c
OBJ = ${OBJ:.c=.o}
DEP = ${OBJ:.o=.d}

all: main

main: ${OBJs}
	make -C ../Image
	$(CC) OCR.c main.c -c
	$(CC) -o main ${OBJS}

clean:
	${RM} ${OBJS} ${DEPS} main
	make -C ../Image clean

-include ${DEPS}

# END
