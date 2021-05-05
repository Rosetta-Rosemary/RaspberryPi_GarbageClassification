AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_LIST_DIR} DIR_SRCS)

find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Network CONFIG REQUIRED)
find_package(Qt5Core CONFIG REQUIRED)
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories("${CMAKE_CURRENT_LIST_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/LogService")
include_directories("${PROJECT_SOURCE_DIR}/KeywordAnalsys")
include_directories("${PROJECT_SOURCE_DIR}/Classify")
include_directories("${PROJECT_SOURCE_DIR}/XmlReader")
include_directories("${PROJECT_SOURCE_DIR}/KeywordAnalsys")
include_directories("${PROJECT_SOURCE_DIR}/ClientTarget")
include_directories("${PROJECT_SOURCE_DIR}/Network")


add_library(ClientProgram_shared SHARED ${DIR_SRCS})

if(UNIX)    # 用于UNIX平台开发
   
else()      # 用于win32平台开发
    target_link_libraries(ClientProgram_shared -lwsock32)
endif()
target_link_libraries(ClientProgram_shared ${PYTHON_LIBRARIES})
target_link_libraries(ClientProgram_shared Qt5::Core)
target_link_libraries(ClientProgram_shared Qt5::Widgets)
target_link_libraries(ClientProgram_shared Qt5::Network)
target_link_libraries(ClientProgram_shared -lpthread)
target_link_libraries(ClientProgram_shared LogService_shared)
target_link_libraries(ClientProgram_shared KeywordAnalsys_shared)
target_link_libraries(ClientProgram_shared ClientTarget_shared)
target_link_libraries(ClientProgram_shared Network_shared)