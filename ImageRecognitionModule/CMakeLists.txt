AUX_SOURCE_DIRECTORY(. DIR_SRCS)

find_package(Qt5Network CONFIG REQUIRED)
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories("./")
include_directories("${PROJECT_SOURCE_DIR}/PythonInterface")
include_directories("${PROJECT_SOURCE_DIR}/udpSocket")

add_executable(ImageRecognitionModule ImageRecognitionModule.cpp ModelControl.hpp)

target_link_libraries(ImageRecognitionModule Qt5::Network)
target_link_libraries(ImageRecognitionModule ${PYTHON_LIBRARIES})
target_link_libraries(ImageRecognitionModule PythonInterface_shared)
target_link_libraries(ImageRecognitionModule -lpthread)
target_link_libraries(ImageRecognitionModule UdpSocket_shared)
