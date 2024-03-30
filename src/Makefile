CC = gcc
CW = x86_64-w64-mingw32-gcc
SRCS = *.c
HEADERS = *.h
FLAGS = -Ofast -std=c2x -march=native -Wall -Wextra
EXE = nibble

.PHONY: all format clean

all:
	$(CC) $(FLAGS) $(SRCS) -o $(EXE)
	$(CW) $(FLAGS) $(SRCS) -o $(EXE)

format:
	clang-format -i $(SRCS) $(HEADERS) -style=file

debug:
	$(CC) -g -std=c2x $(SRCS) -o $(EXE)

clean:
	rm -f nibble nibble.exe
