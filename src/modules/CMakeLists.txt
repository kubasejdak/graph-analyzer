#=== MODULES ===

# list of source files
SET(module_manager_src
	ModuleManager.cpp
   )

add_library(module_manager_lib ${module_manager_src})

# BinaryInput
SET(binary_input_src
	AbstractModule.cpp
	AbstractInput.cpp
	BinaryInput.cpp
   )

add_library(binary_input_lib ${binary_input_src})