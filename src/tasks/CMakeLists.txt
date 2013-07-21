#=== TASKS ===

add_subdirectory(analyze)
add_subdirectory(group)
add_subdirectory(export)

set(TASKS_SRC
    ITask.cpp
    TaskManager.cpp
    TaskResolver.cpp
    TaskQueue.cpp
)

add_library(tasks ${TASKS_SRC})

target_link_libraries(tasks analyze-task)
target_link_libraries(tasks group-task)
target_link_libraries(tasks export-task)
if(${MACHINE} STREQUAL ${DEVELOPMENT_MACHINE})
    qt5_use_modules(tasks Xml)
    qt5_use_modules(tasks Core)
    qt5_use_modules(tasks Sql)
else()
    target_link_libraries(tasks ${QT_LIBRARIES})
endif()
