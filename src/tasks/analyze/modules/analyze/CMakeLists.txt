#=== ANALYZE SUBMODULES ===

set(ANALYZE_SUBMODULES_SRC
    IAnalyze.h
    SyscallAnalyze.cpp
    GraphHash.cpp
    LoopDetector.cpp
)

add_library(analyze-submodules ${ANALYZE_SUBMODULES_SRC})

target_link_libraries(analyze-submodules utils)
if(${MACHINE} STREQUAL ${DEVELOPMENT_MACHINE})
    qt5_use_modules(analyze-submodules Sql)
else()
    target_link_libraries(analyze-submodules ${QT_LIBRARIES})
endif()
