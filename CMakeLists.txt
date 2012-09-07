# === ROOT ===

# required cmake version
cmake_minimum_required(VERSION 2.6)

# project name
project(graph-analyzer)

# options passed to the compiler
add_definitions(-g -Wall -fPIC)

# Qt4
find_package(Qt4 REQUIRED)
set(QT_USE_QTSQL TRUE)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

# Boost
find_package(Boost COMPONENTS program_options python REQUIRED)

#Python
find_package(PythonLibs REQUIRED)

# Libemu
link_directories(/opt/libemu/lib)

# include
include_directories(src)
include_directories(/opt/libemu/include)
include_directories(${Qt4_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIR})
include_directories(${PYTHON_INCLUDE_DIRS})


# source path
add_subdirectory(src)
