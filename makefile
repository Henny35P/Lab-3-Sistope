.DEFAULT_GOAL := install
CC = gcc
CFLAGS = -g -Wall  # Flag to pass to gcc

install:game lab2

game: game.c game.h
	$(CC) $(CFLAGS) -c game.c
	
lab2: lab2.c
	$(CC) $(CFLAGS) -o lab2.o lab2.c queue.o game.o

clean:

		rm -f *.o *.out *.txt

