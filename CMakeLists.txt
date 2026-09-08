cmake_minimum_required(VERSION 4.1)
project(Project C)

set(CMAKE_C_STANDARD 11)

add_executable(Project
        gradient.c
        gradient_sol.c)
