AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_LIST_DIR} DIR_SRCS)

include_directories("${CMAKE_CURRENT_LIST_DIR}/")

find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Xml CONFIG REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}/LogService")

add_library(XmlReader_shared SHARED ${DIR_SRCS})
target_link_libraries(XmlReader_shared  Qt5::Core 
                                        Qt5::Xml
                                        LogService_shared)

SET_TARGET_PROPERTIES (XmlReader_shared PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES (XmlReader_shared PROPERTIES VERSION 1.1 SOVERSION 1)