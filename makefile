CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -g -pthread
OBJECTS = matrix.o matrixFun.o

.PHONY: all
all: prog

prog:$(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o fun

matrix.o: matrix.c
	 $(CC) $(CFLAGS) matrix.c -c

matrixFun.o: matrixFun.c
	 $(CC) $(CFLAGS) matrixFun.c -c

.Phony: clean
clean:
	rm -f $(OBJECTS) fun
