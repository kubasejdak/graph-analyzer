#=== GROUP TASK MODULES ===

add_subdirectory(algorithms)
add_subdirectory(input)
add_subdirectory(output)

set(GROUP_MODULES_SRC
    IModule.h
    ModulesManager.cpp
)

add_library(group-modules ${GROUP_MODULES_SRC} )

target_link_libraries(group-modules core)
target_link_libraries(group-modules group-algorithms-submodules)
target_link_libraries(group-modules group-input-submodules)
target_link_libraries(group-modules group-output-submodules)
if(${MACHINE} STREQUAL ${DEVELOPMENT_MACHINE})
        qt5_use_modules(group-modules Core)
else()
        target_link_libraries(group-modules ${QT_LIBRARIES})
endif()
