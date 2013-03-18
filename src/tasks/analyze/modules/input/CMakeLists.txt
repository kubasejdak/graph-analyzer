#=== INPUT SUBMODULES ===

set(INPUT_SUBMODULES_SRC
    IInput.h
	BinaryInput.cpp
    PcapInput.cpp
)

add_library(input-submodules ${INPUT_SUBMODULES_SRC})
