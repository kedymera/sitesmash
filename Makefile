SHELL=bash
CC=gcc
FLAGS=-Wall -Wextra -pedantic -g -fsanitize=address
LINKFLAGS=

all: build/bms.o convert consume test

convert: build/convert.o
	$(CC) $(FLAGS) -o build/$@ $< build/bms.o $(LINKFLAGS)

consume: build/consume.o
	$(CC) $(FLAGS) -o build/$@ $< build/bms.o $(LINKFLAGS)

test: build/test.o
	$(CC) $(FLAGS) -o build/$@ $< build/bms.o $(LINKFLAGS)

build/%.o: %.c
	$(CC) $(FLAGS) -c -o $@ $<

clean:
	rm -rf build/*
