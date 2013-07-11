# === GROUP TASK ALGORITHMS SUBMODULES ===

set(GROUP_TASK_ALGORITHMS_SUBMODULES_SRC
    IAlgorithm.h
    AlgorithmContext.cpp
    AlgorithmFactory.cpp
    SymetricProbability.cpp
)

add_library(group-algorithms-submodules ${GROUP_TASK_ALGORITHMS_SUBMODULES_SRC})

if(${MACHINE} STREQUAL ${DEVELOPMENT_MACHINE})
    qt5_use_modules(group-algorithms-submodules Core)
else()
    target_link_libraries(group-algorithms-submodules ${QT_LIBRARIES})
endif()
