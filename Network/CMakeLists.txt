AUX_SOURCE_DIRECTORY(. DIR_SRCS)

set(libNetwork  ./Network.cpp 
                ./udpClient.cpp 
                ./udpServer.cpp 
                ./tcpServer.cpp
                ./GetIpAndMac.hpp)

find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Network CONFIG REQUIRED)
find_package(Qt5Core CONFIG REQUIRED)
find_package(PythonLibs REQUIRED)

include_directories("./")
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories("${PROJECT_SOURCE_DIR}/Classify")

include_directories("${PROJECT_SOURCE_DIR}/GetIpAndMac")
include_directories("${PROJECT_SOURCE_DIR}/LogService")
include_directories("${PROJECT_SOURCE_DIR}/KeywordAnalsys")

add_library(Network_shared SHARED ${libNetwork})

if(UNIX)    # 用于UNIX平台开发
    target_link_libraries(Network_shared Qt5::Core)
    target_link_libraries(Network_shared Qt5::Widgets)
    target_link_libraries(Network_shared Qt5::Network)

    target_link_libraries(Network_shared Classify_shared)
    target_link_libraries(Network_shared LogService_shared)
    target_link_libraries(Network_shared XmlReader_shared)
    target_link_libraries(Network_shared KeywordAnalsys_shared)    
else()      # 用于win32平台开发
    target_link_libraries(Network_shared -lwsock32)
    target_link_libraries(Network_shared Qt5::Core)
    target_link_libraries(Network_shared Qt5::Widgets)
    target_link_libraries(Network_shared Qt5::Network)

    target_link_libraries(Network_shared Classify_shared)
    target_link_libraries(Network_shared LogService_shared)
    target_link_libraries(Network_shared XmlReader_shared)
    target_link_libraries(Network_shared KeywordAnalsys_shared)
endif()

SET_TARGET_PROPERTIES (Network_shared PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES (Network_shared PROPERTIES VERSION 1.1 SOVERSION 1)