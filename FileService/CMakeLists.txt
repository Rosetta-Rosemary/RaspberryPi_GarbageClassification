AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_LIST_DIR} DIR_SRCS)

find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Core CONFIG REQUIRED)

include_directories("${CMAKE_CURRENT_LIST_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/LogService")
include_directories("${PROJECT_SOURCE_DIR}/KeywordAnalsys")


add_library(FileService_shared SHARED ${DIR_SRCS})

if(UNIX)    # 用于UNIX平台开发
   
else()      # 用于win32平台开发
    target_link_libraries(FileService_shared -lwsock32)
endif()
target_link_libraries(FileService_shared Qt5::Core)
target_link_libraries(FileService_shared Qt5::Widgets)
target_link_libraries(FileService_shared LogService_shared)
target_link_libraries(FileService_shared KeywordAnalsys_shared)
