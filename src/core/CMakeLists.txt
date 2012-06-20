# === CORE ===

# core library
set(CORE_SRC
	CoreSystem.cpp
	EmulationSystem.cpp
	AnalysisSystem.cpp
	EmulationUnit.cpp
	ShellcodeSample.cpp
	ShellcodeInfo.cpp
	Graph.cpp)

add_library(core ${CORE_SRC})