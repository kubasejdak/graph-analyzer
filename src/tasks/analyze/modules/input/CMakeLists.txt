#=== INPUT SUBMODULES ===

set(INPUT_SUBMODULES_SRC
    IInput.h
    BinaryInput.cpp
    PcapInput.cpp
)

add_library(input-submodules ${INPUT_SUBMODULES_SRC})

if(${MACHINE} STREQUAL ${DEVELOPMENT_MACHINE})
    qt5_use_modules(input-submodules Core)
    qt5_use_modules(input-submodules Sql)
else()
    target_link_libraries(input-submodules ${QT_LIBRARIES})
endif()
