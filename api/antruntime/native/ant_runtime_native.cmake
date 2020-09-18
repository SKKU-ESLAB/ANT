get_filename_component(MODULE_DIR ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)

set(MODULE_NAME "antruntime")

add_subdirectory(${MODULE_DIR}/internal/ ${CMAKE_BINARY_DIR}/out/antruntime/)

list(APPEND EXTERNAL_LIBS ant_runtime_native)