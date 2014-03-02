CC=gcc
CC_FLAGS=-Wall -pedantic -ansi -O99 -Werror
LIB=-lSDL -lSDL_image

SOURCES=$(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

all:	$(OBJECTS)
	$(CC) $(OBJECTS) -o ../bin/cfviz $(LIB)

%.o:	%.c
	$(CC) -c $(CC_FLAGS) $< -o $@ $(LIB)

clean:
	rm -f $(OBJECTS)