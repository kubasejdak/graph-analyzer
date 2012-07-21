#=== MODULES ===

# BinaryInput
set(BINARY_INPUT_SRC
	../AbstractModule.cpp
	AbstractInput.cpp
	BinaryInput.cpp)

add_library(binary_input ${BINARY_INPUT_SRC})