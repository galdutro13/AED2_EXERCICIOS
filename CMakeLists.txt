cmake_minimum_required(VERSION 3.24)
project(countLoops C)

set(CMAKE_C_STANDARD 11)

add_executable(countLoops main.c grafo_datastruct.h busca_emLargura.c busca_emLargura.h gqueue.c gqueue.h stack.c stack.h)
