AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_LIST_DIR} DIR_SRCS)
include_directories("${CMAKE_CURRENT_LIST_DIR}/")

find_package(Qt5Core CONFIG REQUIRED)

add_library(LogService_shared SHARED ${DIR_SRCS})
target_link_libraries(LogService_shared Qt5::Core)

SET_TARGET_PROPERTIES (LogService_shared PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES (LogService_shared PROPERTIES VERSION 1.1 SOVERSION 1)
                                        