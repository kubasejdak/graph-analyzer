#=== MODULES ===

# specialized modules source path
add_subdirectory(analyze)
add_subdirectory(input)
add_subdirectory(output)

# ModuleManager
set(MODULES_MANAGER_SRC
	ModuleManager.cpp)

add_library(modules ${MODULES_MANAGER_SRC})

target_link_libraries(modules analyze)
target_link_libraries(modules input)
target_link_libraries(modules output)
