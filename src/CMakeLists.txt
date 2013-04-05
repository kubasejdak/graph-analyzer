# source paths
add_subdirectory(core)
add_subdirectory(tasks)
add_subdirectory(utils)

# output settings
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/lib)

# project targets
add_executable(graph-analyzer main.cpp)

add_library(GraphAnalyzerCpp SHARED core/CoreSystem.cpp)
set_target_properties(GraphAnalyzerCpp PROPERTIES PREFIX "")

# link with modules 
target_link_libraries(graph-analyzer core)
target_link_libraries(graph-analyzer tasks)
target_link_libraries(graph-analyzer utils)

target_link_libraries(GraphAnalyzerCpp core)
target_link_libraries(GraphAnalyzerCpp tasks)
target_link_libraries(GraphAnalyzerCpp utils)

# link with external libraries
target_link_libraries(graph-analyzer ${QT_LIBRARIES})
target_link_libraries(graph-analyzer ${Boost_LIBRARIES})
target_link_libraries(graph-analyzer emu)

target_link_libraries(GraphAnalyzerCpp ${QT_LIBRARIES})
target_link_libraries(GraphAnalyzerCpp ${Boost_LIBRARIES})
target_link_libraries(GraphAnalyzerCpp emu)

# post compile
add_custom_command(TARGET graph-analyzer POST_BUILD COMMAND ${CMAKE_HOME_DIRECTORY}/bin/graph-analyzer --export-status)

# install
install(PROGRAMS ${CMAKE_HOME_DIRECTORY}/bin/graph-analyzer DESTINATION /usr/bin)
