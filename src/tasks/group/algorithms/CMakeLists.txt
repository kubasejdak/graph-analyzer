# === GROUP ALGORITHMS ===

set(GROUP_ALGORITHMS_SRC
    IAlgorithm.h
    AlgorithmContext.cpp
    AlgorithmFactory.cpp
    SymetricProbability.cpp
)

add_library(group-algorithms ${GROUP_ALGORITHMS_SRC})

if(${MACHINE} STREQUAL ${DEVELOPMENT_MACHINE})
    qt5_use_modules(group-algorithms Core)
else()
    target_link_libraries(group-algorithms ${QT_LIBRARIES})
endif()
