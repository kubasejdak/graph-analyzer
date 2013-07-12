# === CORE ===

set(CORE_SRC
    version.h
    CoreSystem.cpp
    ExploitSample.cpp
    ExploitGroup.cpp
    ExploitInfo.cpp
    Graph.cpp
    Options.cpp
)

add_library(core ${CORE_SRC})

target_link_libraries(core utils)
if(${MACHINE} STREQUAL ${DEVELOPMENT_MACHINE})
    qt5_use_modules(core Xml)
else()
    target_link_libraries(core ${QT_LIBRARIES})
endif()
