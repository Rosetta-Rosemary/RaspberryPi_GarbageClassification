AUX_SOURCE_DIRECTORY(. DIR_SRCS)

set(libKeywordAnalsys ./KeywordAnalsys.cpp ./KeywordAnalsysTaskMgr.cpp ./Signal.hpp)

include_directories("./")

find_package(Qt5Core CONFIG REQUIRED)
include_directories("${PROJECT_SOURCE_DIR}/LogService")
add_library(KeywordAnalsys_shared SHARED ${libKeywordAnalsys})
#add_library( PythonInterface_static STATIC ${libPythonInterface})
target_link_libraries(KeywordAnalsys_shared Qt5::Core)
target_link_libraries(KeywordAnalsys_shared LogService_shared)

# 3、cmake在构建一个新的target时，会尝试清理掉其他使用这个名字的库，
# 因此，在构建libhello.a时，就会清理掉libhello.so
SET_TARGET_PROPERTIES (KeywordAnalsys_shared PROPERTIES CLEAN_DIRECT_OUTPUT 1)
# 4、按照规则，动态库是应该包含一个版本号的，
# VERSION指代动态库版本，SOVERSION指代API版本。
SET_TARGET_PROPERTIES (KeywordAnalsys_shared PROPERTIES VERSION 1.1 SOVERSION 1)
