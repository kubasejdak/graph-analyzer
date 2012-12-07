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
	InstructionSplitter.cpp
	FileAnalyser.cpp
	SystemLogger.cpp
	Options.cpp
	DatabaseManager.cpp
    Toolbox.cpp)

add_library(core ${CORE_SRC})
