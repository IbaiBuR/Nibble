cmake_minimum_required(VERSION 3.27)
project(Nibble C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_FLAGS_RELEASE "-O3 -march=native -funroll-loops -Wall -Wextra")

file(GLOB SRCS "src/*.c" "src/*.h")

add_executable(Nibble ${SRCS})

target_link_options(Nibble PRIVATE)
