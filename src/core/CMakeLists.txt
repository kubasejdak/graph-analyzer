# === CORE ===

# core library
set(CORE_SRC
    version.h
	CoreSystem.cpp
	EmulationSystem.cpp
	AnalysisSystem.cpp
	EmulationUnit.cpp
    ExploitSample.cpp
    ExploitInfo.cpp
	Graph.cpp
    Options.cpp
    Experiment.cpp
    RPCTagResolver.cpp)

add_library(core ${CORE_SRC})
