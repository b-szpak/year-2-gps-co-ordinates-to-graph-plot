cmake_minimum_required(VERSION 3.16)
project(Project C)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
set(CMAKE_C_STANDARD 11)

add_executable(Project main.c menu.c menu.h configuration.c configuration.h gps_plot.c gps_plot.h)