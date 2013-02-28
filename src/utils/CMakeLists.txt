# === UTILS ===

# utils library
set(UTILS_SRC
	InstructionSplitter.cpp
    FileTypeAnalyzer.cpp
	SystemLogger.cpp
	DatabaseManager.cpp
    Toolbox.cpp
	XMLParser.cpp)

add_library(utils ${UTILS_SRC})
