CC = gcc
CFLAGS = -Wall -std=c99 -pthread
SRC = $(wildcard *.c)
EXE = $(subst .c,,$(SRC))

all: $(EXE)
clean:
	rm -f $(EXE)
