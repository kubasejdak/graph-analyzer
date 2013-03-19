#=== ANALYZE TASK MODULES ===

add_subdirectory(analyze)
add_subdirectory(input)
add_subdirectory(output)

set(ANALYZE_MODULES_SRC
    IModule.h
    ModuleManager.cpp
)

add_library(analyze-modules ${ANALYZE_MODULES_SRC} )

target_link_libraries(analyze-modules analyze-submodules)
target_link_libraries(analyze-modules input-submodules)
target_link_libraries(analyze-modules output-submodules)
