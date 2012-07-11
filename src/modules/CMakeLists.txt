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

# SyscallAnalyze
set(SYSCALL_ANALYZE_SRC
	AbstractModule.cpp
	AbstractAnalyze.cpp
	SyscallAnalyze.cpp)

add_library(syscall_analyze ${SYSCALL_ANALYZE_SRC})