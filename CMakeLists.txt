# === ROOT ===

# required cmake version
cmake_minimum_required(VERSION 2.8)

# project name
project(graph-analyzer)

# options passed to the compiler
add_definitions(-g -Wall -fPIC)

# Qt4
find_package(Qt4 COMPONENTS QtCore QtXml QtSql REQUIRED)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

# Boost
find_package(Boost COMPONENTS program_options REQUIRED)

# Libemu
link_directories(/opt/libemu/lib)

# include
include_directories(src)
include_directories(/opt/libemu/include)
include_directories(${Qt4_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIR})

# source path
add_subdirectory(src)
