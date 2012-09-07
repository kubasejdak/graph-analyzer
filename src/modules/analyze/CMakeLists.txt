#=== ANALYZE ===

set(ANALYZE_SRC
	AbstractAnalyze.cpp
	SyscallAnalyze.cpp
	GraphHash.cpp
	LoopDetector.cpp
	../../core/InstructionSplitter.cpp)

add_library(analyze ${ANALYZE_SRC})