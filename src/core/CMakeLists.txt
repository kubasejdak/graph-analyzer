# === CORE ===

# core library
set(CORE_SRC
	CoreSystem.cpp
	EmulationSystem.cpp
	AnalysisSystem.cpp
	EmulationUnit.cpp
    ExploitSample.cpp
    ExploitInfo.cpp
	Graph.cpp
	Dot.cpp
	Options.cpp)

add_library(core ${CORE_SRC})
