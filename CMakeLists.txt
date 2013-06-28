# === ROOT ===

# required cmake version
#cmake_minimum_required(VERSION 2.8.10)
cmake_minimum_required(VERSION 2.8)

# project name
project(graph-analyzer)

# options passed to the compiler
add_definitions(-g -Wall -fPIC -std=c++11)

# environment definitions
set(DEVELOPMENT_MACHINE "development")
set(TARGET_MACHINE "target")

# Qt
if(${CMAKE_VERSION} STREQUAL "2.8.10.1")
    message("-- Development environment: using Qt5")
    find_package(Qt5Core REQUIRED)
    find_package(Qt5Xml REQUIRED)
    find_package(Qt5Sql REQUIRED)
    set(MACHINE ${DEVELOPMENT_MACHINE})
else()
    message("-- Target environment: using Qt4")
    find_package(Qt4 COMPONENTS QtCore QtXml QtSql REQUIRED)
    include(${QT_USE_FILE})
    add_definitions(${QT_DEFINITIONS})
    include_directories(${Qt4_INCLUDE_DIRS})
    set(MACHINE ${TARGET_MACHINE})
endif()

# Boost
find_package(Boost COMPONENTS program_options REQUIRED)

# Libemu
link_directories(/opt/libemu/lib)

# include
include_directories(src)
include_directories(/opt/libemu/include)

# source path
add_subdirectory(src)
