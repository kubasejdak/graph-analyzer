# === GROUP TASK INPUT SUBMODULES ===

set(GROUP_TASK_INPUT_SUBMODULES_SRC
    IInput.h
    DatabaseInput.cpp
)

add_library(group-input-submodules ${GROUP_TASK_INPUT_SUBMODULES_SRC})

if(${MACHINE} STREQUAL ${DEVELOPMENT_MACHINE})
    qt5_use_modules(group-input-submodules Core)
    qt5_use_modules(group-input-submodules Sql)
    qt5_use_modules(group-input-submodules Xml)
else()
    target_link_libraries(group-input-submodules ${QT_LIBRARIES})
endif()
