cmake_minimum_required(VERSION 3.26)
project(sorce C)

set(CMAKE_C_STANDARD 11)

add_library(sorce fork.c
        exec.c
        wait.c
        pipe.c
        signals.c)
