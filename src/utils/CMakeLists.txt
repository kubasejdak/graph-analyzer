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
if(${MACHINE} STREQUAL ${DEVELOPMENT_MACHINE})
    qt5_use_modules(utils Xml)
    qt5_use_modules(utils Sql)
else()
    target_link_libraries(utils ${QT_LIBRARIES})
endif()
