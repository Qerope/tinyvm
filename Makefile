CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -O2 -g

SRC = main.c vm.c asm.c
OBJ = $(SRC:.c=.o)

tinyvm: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

%.o: %.c vm.h
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	rm -f tinyvm $(OBJ)
