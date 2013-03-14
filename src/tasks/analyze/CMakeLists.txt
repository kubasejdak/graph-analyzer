# === ANALYZE TASK ===

# analyze-task library
set(ANALYZE_TASK_SRC
    AnalyzeTask.cpp
	EmulationSystem.cpp
	AnalysisSystem.cpp
	EmulationUnit.cpp)

add_library(analyze-task ${ANALYZE_TASK_SRC})
