AUX_SOURCE_DIRECTORY(. DIR_SRCS)

set(libLogService ./LogService.cpp)

include_directories("./")

find_package(Qt5Core CONFIG REQUIRED)

add_library(LogService_shared SHARED ${libLogService})
target_link_libraries(LogService_shared Qt5::Core)

SET_TARGET_PROPERTIES (LogService_shared PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES (LogService_shared PROPERTIES VERSION 1.1 SOVERSION 1)
                                        