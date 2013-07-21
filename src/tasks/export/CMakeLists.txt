#=== EXPORT TASK ===

add_subdirectory(modules)

set(EXPORT_TASK_SRC
    ExportTask.cpp
)

add_library(export-task ${EXPORT_TASK_SRC})

target_link_libraries(export-task export-modules)
target_link_libraries(export-task core)
target_link_libraries(export-task utils)
if(${MACHINE} STREQUAL ${DEVELOPMENT_MACHINE})
    qt5_use_modules(export-task Core)
    qt5_use_modules(export-task Sql)
    qt5_use_modules(export-task Xml)
else()
    target_link_libraries(export-task ${QT_LIBRARIES})
endif()
