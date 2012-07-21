#=== MODULES ===

# specialized modules source path
add_subdirectory(analyze)
add_subdirectory(input)
add_subdirectory(output)

# ModuleManager
set(MODULE_MANAGER_SRC
	ModuleManager.cpp
	AbstractModule.cpp)

add_library(module_manager ${MODULE_MANAGER_SRC})