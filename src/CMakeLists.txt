# source paths
add_subdirectory(core)
add_subdirectory(modules)

# output settings
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/lib)

# project targets
add_executable(graph-analyzer main.cpp)

add_library(GraphAnalyzerCpp SHARED core/CoreSystem.cpp)
set_target_properties(GraphAnalyzerCpp PROPERTIES PREFIX "")

# link with modules 
target_link_libraries(graph-analyzer core)
target_link_libraries(graph-analyzer modules)

target_link_libraries(GraphAnalyzerCpp core)
target_link_libraries(GraphAnalyzerCpp modules)

# link with external libraries
target_link_libraries(graph-analyzer ${QT_LIBRARIES})
target_link_libraries(graph-analyzer ${Boost_LIBRARIES})
target_link_libraries(graph-analyzer emu)
target_link_libraries(graph-analyzer magic)

target_link_libraries(GraphAnalyzerCpp ${QT_LIBRARIES})
target_link_libraries(GraphAnalyzerCpp ${Boost_LIBRARIES})
target_link_libraries(GraphAnalyzerCpp emu)
target_link_libraries(GraphAnalyzerCpp magic)

# post compile
add_custom_command(TARGET graph-analyzer POST_BUILD COMMAND ${CMAKE_HOME_DIRECTORY}/bin/graph-analyzer --update-db)

# install
install(PROGRAMS ${CMAKE_HOME_DIRECTORY}/bin/graph-analyzer DESTINATION /usr/bin)
