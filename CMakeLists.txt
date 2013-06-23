# === ROOT ===

# required cmake version
cmake_minimum_required(VERSION 2.8.10)

# project name
project(graph-analyzer)

# options passed to the compiler
add_definitions(-g -Wall -fPIC -std=c++11)

# Qt5
find_package(Qt5Core REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5Sql REQUIRED)

# Boost
find_package(Boost COMPONENTS program_options REQUIRED)

# Libemu
link_directories(/opt/libemu/lib)

# include
include_directories(src)
include_directories(/opt/libemu/include)

# source path
add_subdirectory(src)
