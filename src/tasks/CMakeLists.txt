#=== TASKS ===

add_subdirectory(analyze)
add_subdirectory(group)

set(TASKS_SRC
	ITask.cpp
	TaskManager.cpp
	TaskResolver.cpp
    TaskQueue.cpp
)

add_library(tasks ${TASKS_SRC})

target_link_libraries(tasks analyze-task)
target_link_libraries(tasks group-task)
