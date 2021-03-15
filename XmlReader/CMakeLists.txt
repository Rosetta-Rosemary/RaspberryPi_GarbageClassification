AUX_SOURCE_DIRECTORY(. DIR_SRCS)

set(libXmlReader ./XmlReader.cpp)

include_directories("./")

find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Xml CONFIG REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}/LogService")

add_library(XmlReader_shared SHARED ${libXmlReader})
target_link_libraries(XmlReader_shared  Qt5::Core 
                                        Qt5::Xml
                                        LogService_shared)

SET_TARGET_PROPERTIES (XmlReader_shared PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES (XmlReader_shared PROPERTIES VERSION 1.1 SOVERSION 1)