cmake_minimum_required(VERSION 3.27)
project(Nibble C)

set(CMAKE_C_STANDARD 23)

add_executable(Nibble src/nibble.c
        src/types.h
        src/bitboard.h
        src/bitboard.c
        src/util.h
        src/util.c
        src/board.c
        src/board.h
        src/movegen.c
        src/movegen.h
        src/move.c
        src/move.h
)
