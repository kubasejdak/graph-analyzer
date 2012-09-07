# source paths
add_subdirectory(core)
add_subdirectory(modules)

# project targets
add_executable(graph-analyzer main.cpp toolbox.cpp)
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_HOME_DIRECTORY}/bin")

add_library(GraphAnalyzerPython SHARED core/GraphAnalyzerPython.cpp toolbox.cpp)
set_target_properties(GraphAnalyzerPython PROPERTIES PREFIX "")
set_target_properties(GraphAnalyzerPython PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_HOME_DIRECTORY}/bin")

# link with modules 
target_link_libraries(graph-analyzer emu)
target_link_libraries(graph-analyzer core)
target_link_libraries(graph-analyzer modules)

target_link_libraries(GraphAnalyzerPython emu)
target_link_libraries(GraphAnalyzerPython core)
target_link_libraries(GraphAnalyzerPython modules)

# link with external libraries
target_link_libraries(graph-analyzer ${QT_LIBRARIES})
target_link_libraries(graph-analyzer ${Boost_LIBRARIES})

target_link_libraries(GraphAnalyzerPython ${QT_LIBRARIES})
target_link_libraries(GraphAnalyzerPython ${Boost_LIBRARIES})
target_link_libraries(GraphAnalyzerPython ${PYTHON_LIBRARIES})