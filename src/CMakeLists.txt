# source paths
add_subdirectory(core)
add_subdirectory(modules)

# output settings
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_HOME_DIRECTORY}/bin")
set(LIBRARY_OUTPUT_PATH "${CMAKE_HOME_DIRECTORY}/lib")

# project targets
add_executable(graph-analyzer main.cpp toolbox.cpp)

add_library(GraphAnalyzerPython SHARED core/GraphAnalyzerPython.cpp toolbox.cpp)
set_target_properties(GraphAnalyzerPython PROPERTIES PREFIX "")

add_library(GraphAnalyzerCpp SHARED core/CoreSystem.cpp toolbox.cpp)
set_target_properties(GraphAnalyzerCpp PROPERTIES PREFIX "")

# link with modules 
target_link_libraries(graph-analyzer emu)
target_link_libraries(graph-analyzer core)
target_link_libraries(graph-analyzer modules)

target_link_libraries(GraphAnalyzerPython emu)
target_link_libraries(GraphAnalyzerPython core)
target_link_libraries(GraphAnalyzerPython modules)

target_link_libraries(GraphAnalyzerCpp emu)
target_link_libraries(GraphAnalyzerCpp core)
target_link_libraries(GraphAnalyzerCpp modules)

# link with external libraries
target_link_libraries(graph-analyzer ${QT_LIBRARIES})
target_link_libraries(graph-analyzer ${Boost_LIBRARIES})

target_link_libraries(GraphAnalyzerPython ${QT_LIBRARIES})
target_link_libraries(GraphAnalyzerPython ${Boost_LIBRARIES})
target_link_libraries(GraphAnalyzerPython ${PYTHON_LIBRARIES})

target_link_libraries(GraphAnalyzerCpp ${QT_LIBRARIES})
target_link_libraries(GraphAnalyzerCpp ${Boost_LIBRARIES})