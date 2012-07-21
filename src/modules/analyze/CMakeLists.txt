#=== ANALYZE ===

# SyscallAnalyze
set(SYSCALL_ANALYZE_SRC
	AbstractAnalyze.cpp
	SyscallAnalyze.cpp
	../../core/InstructionSplitter.cpp)

add_library(syscall_analyze ${SYSCALL_ANALYZE_SRC})

# DecoderKey
set(DECODER_KEY_SRC
	AbstractAnalyze.cpp
	DecoderKey.cpp)

add_library(decoder_key ${DECODER_KEY_SRC})

# LoopDetector
set(LOOP_DETECTOR_SRC
	AbstractAnalyze.cpp
	LoopDetector.cpp)

add_library(loop_detector ${LOOP_DETECTOR_SRC})