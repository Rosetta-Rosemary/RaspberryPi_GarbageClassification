AUX_SOURCE_DIRECTORY(. DIR_SRCS)

find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Network CONFIG REQUIRED)
find_package(Qt5Core CONFIG REQUIRED)
find_package(PythonLibs REQUIRED)
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories("./")
include_directories("${PROJECT_SOURCE_DIR}/PythonInterface")
include_directories("${PROJECT_SOURCE_DIR}/Classify")
include_directories("${PROJECT_SOURCE_DIR}/Network")
include_directories("${PROJECT_SOURCE_DIR}/LogService")
include_directories("${PROJECT_SOURCE_DIR}/XmlReader")
include_directories("${PROJECT_SOURCE_DIR}/KeywordAnalsys")

add_executable(ImageRecognitionModule ImageRecognitionModule.cpp ModelControl.hpp)

target_link_libraries(ImageRecognitionModule Qt5::Core)
target_link_libraries(ImageRecognitionModule Qt5::Widgets)
target_link_libraries(ImageRecognitionModule Qt5::Network)
target_link_libraries(ImageRecognitionModule -lpthread)
target_link_libraries(ImageRecognitionModule PythonInterface_shared)
target_link_libraries(ImageRecognitionModule Classify_shared)
target_link_libraries(ImageRecognitionModule Network_shared)
target_link_libraries(ImageRecognitionModule LogService_shared)
target_link_libraries(ImageRecognitionModule XmlReader_shared)
target_link_libraries(ImageRecognitionModule KeywordAnalsys_shared)