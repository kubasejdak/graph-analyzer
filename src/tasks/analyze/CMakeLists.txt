#=== ANALYZE TASK ===

add_subdirectory(modules)

set(ANALYZE_TASK_SRC
    EmulationUnit.cpp
	EmulationSystem.cpp
	AnalyzeTask.cpp
    AnalysisSystem.cpp
)

add_library(analyze-task ${ANALYZE_TASK_SRC})

target_link_libraries(analyze-task analyze-modules)
target_link_libraries(analyze-task core)
target_link_libraries(analyze-task utils)
target_link_libraries(analyze-task emu)
