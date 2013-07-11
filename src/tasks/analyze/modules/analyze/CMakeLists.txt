#=== ANALYZE TASK ANALYZE SUBMODULES ===

set(ANALYZETASK_ANALYZE_SUBMODULES_SRC
    IAnalyze.h
    SyscallAnalyze.cpp
    GraphHash.cpp
    LoopDetector.cpp
)

add_library(analyze-analyze-submodules ${ANALYZETASK_ANALYZE_SUBMODULES_SRC})

target_link_libraries(analyze-analyze-submodules utils)
if(${MACHINE} STREQUAL ${DEVELOPMENT_MACHINE})
	qt5_use_modules(analyze-analyze-submodules Sql)
else()
	target_link_libraries(analyze-analyze-submodules ${QT_LIBRARIES})
endif()
