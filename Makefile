all: onlydot

CC=gcc
CFLAGS=$(shell pkg-config --cflags --libs sdl3)

onlydot: onlydot.c
	$(CC) $(CFLAGS) -o onlydot onlydot.c && ./onlydot

build: onlydot.c
	$(CC) $(CFLAGS) -o onlydot onlydot.c


clean:
	rm -f onlydot
