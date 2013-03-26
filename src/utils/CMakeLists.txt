# === UTILS ===

set(UTILS_SRC
	InstructionSplitter.cpp
    FileTypeAnalyzer.cpp
	SystemLogger.cpp
	DatabaseManager.cpp
    Toolbox.cpp
    XMLParser.cpp
    LoggingStrategy.cpp
    UniqueNameResolver.cpp
    DateRegExp.cpp
)

add_library(utils ${UTILS_SRC})
