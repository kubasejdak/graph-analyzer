# core source path
add_subdirectory(core)

# modules source path
add_subdirectory(modules)

# exe directory
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_HOME_DIRECTORY}/bin")

# libemu directory
link_directories(/opt/libemu/lib)

# compile project
add_executable(graph_analyzer main.cpp toolbox.cpp)

# link with modules 
target_link_libraries(graph_analyzer emu)
target_link_libraries(graph_analyzer core)
target_link_libraries(graph_analyzer module_manager)
target_link_libraries(graph_analyzer binary_input)
target_link_libraries(graph_analyzer pcap_input)
target_link_libraries(graph_analyzer syscall_analyze)
target_link_libraries(graph_analyzer graph_hash)
target_link_libraries(graph_analyzer loop_detector)