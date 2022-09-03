SHELL=bash
CC=gcc
FLAGS=-Wall -Wextra -g -fsanitize=address
LINKFLAGS=

all: build/bms.o cbm

cbm: build/cbm.o
	$(CC) $(FLAGS) -o build/cbm build/cbm.o build/bms.o $(LINKFLAGS)

build/%.o: %.c
	$(CC) $(FLAGS) -c -o $@ $<

clean:
	rm -rf build/*
