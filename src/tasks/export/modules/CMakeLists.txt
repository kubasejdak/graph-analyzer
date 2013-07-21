#=== EXPORT TASK MODULES ===

add_subdirectory(output)

set(EXPORT_MODULES_SRC
    IModule.h
    ModulesManager.cpp
)

add_library(export-modules ${EXPORT_MODULES_SRC} )

target_link_libraries(export-modules export-output-submodules)
if(${MACHINE} STREQUAL ${DEVELOPMENT_MACHINE})
    qt5_use_modules(export-modules Core)
    qt5_use_modules(export-modules Sql)
else()
    target_link_libraries(export-modules ${QT_LIBRARIES})
endif()
