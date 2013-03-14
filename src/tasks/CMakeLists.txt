#=== TASKS ===

# specialized tasks source path
add_subdirectory(analyze)
add_subdirectory(group)

# Tasks
set(TASK_MANAGER_SRC
    ITask.h
	TaskManager.cpp
	TaskResolver.cpp
	TaskQueue.cpp)

add_library(tasks ${TASK_MANAGER_SRC})

target_link_libraries(tasks analyze)
target_link_libraries(tasks group)
