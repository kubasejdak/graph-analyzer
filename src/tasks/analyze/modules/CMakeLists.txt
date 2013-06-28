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
if(${MACHINE} STREQUAL ${DEVELOPMENT_MACHINE})
    qt5_use_modules(analyze-modules Core)
    qt5_use_modules(analyze-modules Sql)
else()
    target_link_libraries(analyze-modules ${QT_LIBRARIES})
endif()
