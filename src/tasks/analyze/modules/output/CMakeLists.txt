#=== OUTPUT SUBMODULES ===

set(OUTPUT_SUBMODULES_SRC
    IOutput.h
    DatabaseOutput.cpp
    ConsoleOutput.cpp
)

add_library(output-submodules ${OUTPUT_SUBMODULES_SRC})
if(${MACHINE} STREQUAL ${DEVELOPMENT_MACHINE})
    qt5_use_modules(output-submodules Sql)
else()
    target_link_libraries(output-submodules ${QT_LIBRARIES})
endif()
