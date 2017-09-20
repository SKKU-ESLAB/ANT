include_directories(./inc)
include_directories(../message/inc)
include_directories(../communication/inc)
include_directories(./libminiunz)
include_directories(/usr/local/xml/include/libxml2)
link_directories(/usr/local/xml/lib)
link_directories(../../out/libs)

add_definitions("`pkg-config --libs --cflags dbus-1 glib-2.0 dbus-glib-1`")

set(CMAKE_CXX_STANDARD 11)
set(ACM_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(LIBMINIUNZ_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libminiunz)

find_library(ANT_LIBMINIUNZ z ${LIBMINIUNZ_DIR}/libminiunz)
add_library(miniunz OBJECT
  ${LIBMINIUNZ_DIR}/miniunz.c
  ${LIBMINIUNZ_DIR}/ioapi.c
  ${LIBMINIUNZ_DIR}/unzip.c)

set(ACM_sources
  ${ACM_SOURCE_DIR}/main.cpp
  ${ACM_SOURCE_DIR}/App.cpp
  ${ACM_SOURCE_DIR}/AppCore.cpp
  ${ACM_SOURCE_DIR}/AppList.cpp)

add_executable(ant-appcore ${ACM_sources} $<TARGET_OBJECTS:miniunz>)

target_link_libraries(ant-appcore
  ant-message ant-cmfw
  sqlite3 xml2 ${ANT_LIBMINIUNZ})

set_target_properties(ant-appcore
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "../../out/bin")
