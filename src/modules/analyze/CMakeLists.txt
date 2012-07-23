#=== ANALYZE ===

# SyscallAnalyze
set(SYSCALL_ANALYZE_SRC
	AbstractAnalyze.cpp
	SyscallAnalyze.cpp
	../../core/InstructionSplitter.cpp)

add_library(syscall_analyze ${SYSCALL_ANALYZE_SRC})

# GraphHash
set(GRAPH_HASH_SRC
	AbstractAnalyze.cpp
	GraphHash.cpp
	../../core/InstructionSplitter.cpp)

add_library(graph_hash ${GRAPH_HASH_SRC})

# LoopDetector
set(LOOP_DETECTOR_SRC
	AbstractAnalyze.cpp
	LoopDetector.cpp)

add_library(loop_detector ${LOOP_DETECTOR_SRC})