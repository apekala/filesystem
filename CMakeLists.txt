cmake_minimum_required(VERSION 3.27)
project(filesystem C)

set(CMAKE_C_STANDARD 23)

add_executable(filesystem main.c
        filesystem.c
        file_operations.c)
