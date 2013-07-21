#=== ANALYZE TASK OUTPUT SUBMODULES ===

set(ANALYZE_TASK_OUTPUT_SUBMODULES_SRC
    IOutput.h
    DatabaseOutput.cpp
    ConsoleOutput.cpp
)

add_library(analyze-output-submodules ${ANALYZE_TASK_OUTPUT_SUBMODULES_SRC})

target_link_libraries(analyze-output-submodules ${Boost_LIBRARIES})
if(${MACHINE} STREQUAL ${DEVELOPMENT_MACHINE})
        qt5_use_modules(analyze-output-submodules Sql)
else()
	target_link_libraries(analyze-output-submodules ${QT_LIBRARIES})
endif()
