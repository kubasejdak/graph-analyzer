# === CORE ===

set(CORE_SRC
    version.h
	CoreSystem.cpp
    ExploitSample.cpp
    ExploitInfo.cpp
	Graph.cpp
    Options.cpp
)

add_library(core ${CORE_SRC})

target_link_libraries(core utils)
qt5_use_modules(core Sql)
qt5_use_modules(core Xml)
qt5_use_modules(core Core)
