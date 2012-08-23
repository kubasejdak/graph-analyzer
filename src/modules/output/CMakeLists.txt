#=== MODULES ===

# DatabaseOutput
set(DATABASE_OUTPUT_SRC
	AbstractOutput.cpp
	DatabaseOutput.cpp)

add_library(database_output ${DATABASE_OUTPUT_SRC} ${QT_LIBRARIES})

# ConsoleOutput
set(CONSOLE_OUTPUT_SRC
	AbstractOutput.cpp
	ConsoleOutput.cpp)

add_library(console_output ${CONSOLE_OUTPUT_SRC})