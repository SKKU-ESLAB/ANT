get_filename_component(MODULE_DIR ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)

set(MODULE_NAME "antml")

add_subdirectory(${MODULE_DIR}/internal/ ${CMAKE_BINARY_DIR}/out/antml/)

list(APPEND EXTERNAL_LIBS ant_ml_internal)