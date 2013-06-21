# === GROUP TASK ===

set(GROUP_TASK_SRC
    GroupTask.cpp
    ../../utils/DatabaseManager.cpp
    GroupManager.cpp
)

add_library(group-task ${GROUP_TASK_SRC})

qt5_use_modules(group-task Core)
qt5_use_modules(group-task Xml)
qt5_use_modules(group-task Sql)
