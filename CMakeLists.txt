# === ROOT ===

# required cmake version
cmake_minimum_required(VERSION 2.6)

# project name
project(graph_analyzer)

# options passed to the compiler
add_definitions(-g -Wall)

# Qt4
find_package(Qt4 REQUIRED)
set(QT_USE_QTSQL TRUE)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

# include
include_directories(src)
include_directories(/opt/libemu/include)
include_directories(${Qt4_INCLUDE_DIRS})

# source path
add_subdirectory(src)
