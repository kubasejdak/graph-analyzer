# core source path
add_subdirectory(core)

# modules source path
add_subdirectory(modules)

# exe directory
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_HOME_DIRECTORY}/bin")

# libemu directory
link_directories(/opt/libemu/lib)

# compile project
add_executable(graph-analyzer main.cpp toolbox.cpp)

# link with modules 
target_link_libraries(graph-analyzer emu)
target_link_libraries(graph-analyzer core)
target_link_libraries(graph-analyzer module_manager)
target_link_libraries(graph-analyzer binary_input)
target_link_libraries(graph-analyzer pcap_input)
target_link_libraries(graph-analyzer syscall_analyze)
target_link_libraries(graph-analyzer graph_hash)
target_link_libraries(graph-analyzer loop_detector)
target_link_libraries(graph-analyzer database_output)
target_link_libraries(graph-analyzer console_output)

# link with external libraries
target_link_libraries(graph-analyzer ${QT_LIBRARIES})
target_link_libraries(graph-analyzer ${Boost_LIBRARIES})