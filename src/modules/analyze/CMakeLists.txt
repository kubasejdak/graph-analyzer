#=== ANALYZE ===

set(ANALYZE_SRC
	AbstractAnalyze.cpp
	SyscallAnalyze.cpp
	GraphHash.cpp
	LoopDetector.cpp
	../../utils/InstructionSplitter.cpp)

add_library(analyze ${ANALYZE_SRC})
