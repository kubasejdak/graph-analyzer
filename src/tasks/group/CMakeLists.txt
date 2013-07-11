# === GROUP TASK ===

add_subdirectory(modules)

set(GROUP_TASK_SRC
    GroupTask.cpp
    GroupManager.cpp
)

add_library(group-task ${GROUP_TASK_SRC})

target_link_libraries(group-task group-modules)
if(${MACHINE} STREQUAL ${DEVELOPMENT_MACHINE})
    qt5_use_modules(group-task Core)
    qt5_use_modules(group-task Xml)
    qt5_use_modules(group-task Sql)
else()
    target_link_libraries(group-task ${QT_LIBRARIES})
endif()
