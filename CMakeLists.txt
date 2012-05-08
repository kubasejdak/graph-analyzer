# required cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# project name
PROJECT(GRAPH_ANALYZER)

# options passed to the compiler
ADD_DEFINITIONS(-g -O2 -Wall -Werror)

#-------------------------------------
# include
#-------------------------------------

INCLUDE_DIRECTORIES(src)

#-------------------------------------
# source
#-------------------------------------

# source path
ADD_SUBDIRECTORY(src bin)
