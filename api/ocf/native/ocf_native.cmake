get_filename_component(MODULE_DIR ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)

set(MODULE_NAME "ocf")

add_subdirectory(${MODULE_DIR}/internal/ ${CMAKE_BINARY_DIR}/out/ocf/)

list(APPEND EXTERNAL_LIBS ocf_native)