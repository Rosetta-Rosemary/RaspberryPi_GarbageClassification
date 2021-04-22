AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_LIST_DIR} DIR_SRCS)

find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Network CONFIG REQUIRED)
find_package(Qt5Core CONFIG REQUIRED)
find_package(PythonLibs REQUIRED)

include_directories("${CMAKE_CURRENT_LIST_DIR}/")
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories("${PROJECT_SOURCE_DIR}/LogService")
include_directories("${PROJECT_SOURCE_DIR}/KeywordAnalsys")
include_directories("${PROJECT_SOURCE_DIR}/XmlReader")

add_library(Classify_shared SHARED ${DIR_SRCS})
target_link_libraries(Classify_shared ${PYTHON_LIBRARIES})
target_link_libraries(Classify_shared Qt5::Core)
target_link_libraries(Classify_shared Qt5::Widgets)
target_link_libraries(Classify_shared LogService_shared)
target_link_libraries(Classify_shared XmlReader_shared)
target_link_libraries(Classify_shared KeywordAnalsys_shared) 

SET_TARGET_PROPERTIES (Classify_shared PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES (Classify_shared PROPERTIES VERSION 1.1 SOVERSION 1)