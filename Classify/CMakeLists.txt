AUX_SOURCE_DIRECTORY(. DIR_SRCS)

set(libClassify ./Classify.cpp)

find_package(Qt5Core CONFIG REQUIRED)

include_directories("./")

add_library(Classify_shared SHARED ${libClassify})
target_link_libraries(Classify_shared Qt5::Core)

SET_TARGET_PROPERTIES (Classify_shared PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES (Classify_shared PROPERTIES VERSION 1.1 SOVERSION 1)