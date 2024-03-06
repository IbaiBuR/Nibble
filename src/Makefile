CC = gcc
CW = x86_64-w64-mingw32-gcc
SRCS = *.c
HEADERS = *.h
FLAGS = -oFast -std=c2x -march=native
EXE = nibble

.PHONY: clean

all:
	$(CC) $(FLAGS) $(SRCS) -o $(EXE)
	$(CW) $(FLAGS) $(SRCS) -o $(EXE)

debug:
	$(CC) $(SRCS) -o $(EXE)
	$(CW) $(SRCS) -o $(EXE)

format:
	clang-format -i $(SRCS) $(HEADERS) -style=file

clean:
	rm -f nibble nibble.exe