# === CORE ===

# core library
set(CORE_SRC
	AnalysisSystem.cpp 
	EmulationSystem.cpp
	EmulationUnit.cpp
	ShellcodeSample.cpp
	ShellcodeInfo.cpp)

add_library(core ${CORE_SRC})