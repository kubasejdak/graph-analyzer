# === ROOT ===

# required cmake version
cmake_minimum_required(VERSION 2.6)

# project name
project(graph_analyzer)

# options passed to the compiler
add_definitions(-g -Wall) #-Werror

# include
include_directories(src)
include_directories(/opt/libemu/include)

# source path
add_subdirectory(src)
