#=== CORE ===

# list of source files
SET(core_src
	main.cpp
	AnalysisSystem.cpp
	EmulationSystem.cpp
	ShellcodeSample.cpp
	ShellcodeInfo.cpp
   )

#-------------------------------------
# compile
#-------------------------------------

ADD_EXECUTABLE(graph_analyzer ${core_src})

#-------------------------------------
# link with modules
#-------------------------------------

# ModuleManager
target_link_libraries(graph_analyzer module_manager_lib)

# BinaryInput
target_link_libraries(graph_analyzer binary_input_lib)