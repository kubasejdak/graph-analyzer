# === UTILS ===

set(UTILS_SRC
	InstructionSplitter.cpp
    FileTypeAnalyzer.cpp
	SystemLogger.cpp
	DatabaseManager.cpp
    Toolbox.cpp
    XMLParser.cpp
    LoggingStrategy.cpp
	StatusExportStrategy.cpp
	DescriptionExportStrategy.cpp
    UniqueNameResolver.cpp
    DateRegExp.cpp
)

add_library(utils ${UTILS_SRC})

target_link_libraries(utils magic)
target_link_libraries(utils ${QT_LIBRARIES})
