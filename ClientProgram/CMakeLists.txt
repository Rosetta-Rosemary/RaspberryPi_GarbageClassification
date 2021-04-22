AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_LIST_DIR} DIR_SRCS)

find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Network CONFIG REQUIRED)
find_package(Qt5Core CONFIG REQUIRED)
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories("${CMAKE_CURRENT_LIST_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/LogService")
include_directories("${PROJECT_SOURCE_DIR}/KeywordAnalsys")


add_executable(ClientProgram ${DIR_SRCS})

if(UNIX)    # 用于UNIX平台开发
   
else()      # 用于win32平台开发
    target_link_libraries(ClientProgram -lwsock32)
endif()
target_link_libraries(ClientProgram ${PYTHON_LIBRARIES})
target_link_libraries(ClientProgram Qt5::Core)
target_link_libraries(ClientProgram Qt5::Widgets)
target_link_libraries(ClientProgram Qt5::Network)
target_link_libraries(ClientProgram -lpthread)
target_link_libraries(ClientProgram LogService_shared)
target_link_libraries(ClientProgram KeywordAnalsys_shared)