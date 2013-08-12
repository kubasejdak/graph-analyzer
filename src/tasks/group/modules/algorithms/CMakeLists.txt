# === GROUP TASK ALGORITHMS SUBMODULES ===

set(GROUP_TASK_ALGORITHMS_SUBMODULES_SRC
    IAlgorithm.h
    AlgorithmContext.cpp
    SymetricSymilarity.cpp
)

add_library(group-algorithms-submodules ${GROUP_TASK_ALGORITHMS_SUBMODULES_SRC})

if(${MACHINE} STREQUAL ${DEVELOPMENT_MACHINE})
    qt5_use_modules(group-algorithms-submodules Core)
    qt5_use_modules(group-algorithms-submodules Xml)
else()
    target_link_libraries(group-algorithms-submodules ${QT_LIBRARIES})
endif()
