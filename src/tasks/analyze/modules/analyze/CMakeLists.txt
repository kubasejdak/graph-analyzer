#=== ANALYZE SUBMODULES ===

set(ANALYZE_SUBMODULES_SRC
    IAnalyze.h
	SyscallAnalyze.cpp
	GraphHash.cpp
	LoopDetector.cpp
    ../../../../utils/InstructionSplitter.cpp
)

add_library(analyze-submodules ${ANALYZE_SUBMODULES_SRC})
