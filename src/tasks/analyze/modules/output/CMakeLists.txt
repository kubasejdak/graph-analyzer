#=== OUTPUT SUBMODULES ===

set(OUTPUT_SUBMODULES_SRC
    IOutput.h
	DatabaseOutput.cpp
    ConsoleOutput.cpp
)

add_library(output-submodules ${OUTPUT_SUBMODULES_SRC})
