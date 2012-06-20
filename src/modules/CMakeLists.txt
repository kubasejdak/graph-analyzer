#=== MODULES ===

# ModuleManager
set(MODULE_MANAGER_SRC
	ModuleManager.cpp)

add_library(module_manager ${MODULE_MANAGER_SRC})

# BinaryInput
set(BINARY_INPUT_SRC
	AbstractModule.cpp
	AbstractInput.cpp
	BinaryInput.cpp)

add_library(binary_input ${BINARY_INPUT_SRC})