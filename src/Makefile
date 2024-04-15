CC = gcc
SRCS = *.c
HEADERS = *.h
FLAGS = -O3 -std=c2x -march=native -funroll-loops -Wall -Wextra
EXE = nibble

.PHONY: all format clean

all:
	$(CC) $(FLAGS) $(SRCS) -o $(EXE)

format:
	clang-format -i $(SRCS) $(HEADERS) -style=file

debug:
	$(CC) -g -std=c2x $(SRCS) -o $(EXE)

clean:
	rm -f nibble
