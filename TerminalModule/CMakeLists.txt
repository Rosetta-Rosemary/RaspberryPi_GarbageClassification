AUX_SOURCE_DIRECTORY(. DIR_SRCS)

find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Network CONFIG REQUIRED)
find_package(Qt5Core CONFIG REQUIRED)
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories("./")
include_directories("${PROJECT_SOURCE_DIR}/LogService")
include_directories("${PROJECT_SOURCE_DIR}/KeywordAnalsys")

add_executable(TerminalModule TerminalNetwork.cpp TerminalModule.hpp TerminalModule.cpp GetIpAndMac.hpp)

if(UNIX)    # 用于UNIX平台开发
   
else()      # 用于win32平台开发
    target_link_libraries(TerminalModule -lwsock32)
endif()
target_link_libraries(TerminalModule ${PYTHON_LIBRARIES})
target_link_libraries(TerminalModule Qt5::Core)
target_link_libraries(TerminalModule Qt5::Widgets)
target_link_libraries(TerminalModule Qt5::Network)
target_link_libraries(TerminalModule -lpthread)
target_link_libraries(TerminalModule LogService_shared)
target_link_libraries(TerminalModule KeywordAnalsys_shared)