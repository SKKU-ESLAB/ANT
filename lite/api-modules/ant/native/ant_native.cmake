get_filename_component(MODULE_DIR ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)

set(MODULE_NAME "ant")

add_subdirectory(${MODULE_DIR}/stream/ ${CMAKE_BINARY_DIR}/out/)

list(APPEND EXTERNAL_LIBS ant_native_stream)