#=== MODULES ===

set(OUTPUT_SRC
	AbstractOutput.cpp
	DatabaseOutput.cpp
	ConsoleOutput.cpp)
	
add_library(output ${OUTPUT_SRC})