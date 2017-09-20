cmake_minimum_required(VERSION 2.8)

add_subdirectory(app)
add_subdirectory(camera)
add_subdirectory(communication)
add_subdirectory(face-detection)
add_subdirectory(sensor)

add_custom_target(
  nil ALL
  COMMENT "Make NIL directory"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
