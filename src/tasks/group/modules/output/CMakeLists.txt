#=== GROUP TASK OUTPUT SUBMODULES ===

set(GROUP_TASK_OUTPUT_SUBMODULES_SRC
    IOutput.h
    DatabaseOutput.cpp
    ConsoleOutput.cpp
)

add_library(group-output-submodules ${GROUP_TASK_OUTPUT_SUBMODULES_SRC})
if(${MACHINE} STREQUAL ${DEVELOPMENT_MACHINE})
    qt5_use_modules(group-output-submodules Core)
    qt5_use_modules(group-output-submodules Sql)
else()
        target_link_libraries(group-output-submodules ${QT_LIBRARIES})
endif()
