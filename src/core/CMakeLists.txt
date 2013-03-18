# === CORE ===

set(CORE_SRC
    version.h
	CoreSystem.cpp
    ExploitSample.cpp
    ExploitInfo.cpp
	Graph.cpp
    Options.cpp
    Experiment.cpp
)

add_library(core ${CORE_SRC})
