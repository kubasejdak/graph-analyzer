#=== ANALYZE TASK INPUT SUBMODULES ===

set(ANALYZE_TASK_INPUT_SUBMODULES_SRC
    IInput.h
    BinaryInput.cpp
    PcapInput.cpp
    IndexFileInput.cpp
)

add_library(analyze-input-submodules ${ANALYZE_TASK_INPUT_SUBMODULES_SRC})

if(${MACHINE} STREQUAL ${DEVELOPMENT_MACHINE})
	qt5_use_modules(analyze-input-submodules Core)
	qt5_use_modules(analyze-input-submodules Sql)
else()
	target_link_libraries(analyze-input-submodules ${QT_LIBRARIES})
endif()
