cmake_minimum_required(VERSION 3.16)
project(lab3 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Часть 1: класс Vector (vector.h, vector.cpp) и main.cpp
add_executable(lab3_part1 part1_vector/main.cpp part1_vector/vector.cpp part1_vector/vector.h)
# input.txt копируется в папку сборки, откуда CLion запускает программу
configure_file(part1_vector/input.txt ${CMAKE_BINARY_DIR}/input.txt COPYONLY)

# Часть 2: наследование
add_executable(lab3_part2 part2_inheritance.cpp)
