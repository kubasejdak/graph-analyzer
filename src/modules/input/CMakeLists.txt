#=== MODULES ===

set(INPUT_SRC
	AbstractInput.cpp
	BinaryInput.cpp
	PcapInput.cpp)

add_library(input ${INPUT_SRC})