#=== INPUT SUBMODULES ===

set(INPUT_SUBMODULES_SRC
    IInput.h
	BinaryInput.cpp
    PcapInput.cpp
)

add_library(input-submodules ${INPUT_SUBMODULES_SRC})

qt5_use_modules(input-submodules Core)
qt5_use_modules(input-submodules Sql)
