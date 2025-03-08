cmake_minimum_required(VERSION 3.30)
project(Mancalamax C)

set(CMAKE_C_STANDARD 11)
add_compile_options(-Wall -Werror)

set(CMAKE_VERBOSE_MAKEFILE ON)

add_executable(Mancalamax mancala/main.c
        utils/LinkedList.c
        utils/LinkedList.h
        mancala/State.c
        mancala/State.h
        mancala/minimax.c
        mancala/minimax.h
)
