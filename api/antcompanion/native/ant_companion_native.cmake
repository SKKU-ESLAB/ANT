get_filename_component(MODULE_DIR ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)

set(MODULE_NAME "antcompanion")

add_subdirectory(${MODULE_DIR}/internal/ ${CMAKE_BINARY_DIR}/out/antcompanion/)

list(APPEND EXTERNAL_LIBS ant_companion_native)