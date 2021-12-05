CC=gcc -fsanitize=address

CPPFLAGS= -MMD `pkg-config --cflags SDL2_image` `pkg-config --cflags gtk+-3.0` 
CFLAGS= -Wall -Wextra -std=c99 -O3  -lm -ldl -lSDL2 -lSDL2_image -D__NO_INLINE__
LDFLAGS= 
LDLIBS= `pkg-config --libs sdl SDL2_image` `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`
SCR = interface.c rotation.c basic_pixel.c resize.c 
OBJS = $(SCR:%.c = %.o)
all: interface

interface: $(OBJS)
	gcc $(CFLAGS) $(LDLIBS) -o $@ $^

clean:

	rm interface
