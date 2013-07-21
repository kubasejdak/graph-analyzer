#=== EXPORT TASK OUTPUT SUBMODULES ===

set(EXPORT_TASK_OUTPUT_SUBMODULES_SRC
    IOutput.h
    SampleExportOutput.cpp
)

add_library(export-output-submodules ${EXPORT_TASK_OUTPUT_SUBMODULES_SRC})

target_link_libraries(export-output-submodules ${Boost_LIBRARIES})
if(${MACHINE} STREQUAL ${DEVELOPMENT_MACHINE})
        qt5_use_modules(export-output-submodules Sql)
else()
	target_link_libraries(export-output-submodules ${QT_LIBRARIES})
endif()
