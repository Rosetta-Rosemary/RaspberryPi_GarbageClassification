cmake_minimum_required(VERSION 3.0.0)
project(DEMO_Project_All VERSION 0.1.0)

#setting with Start VS code Debug
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

#Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
#Instruct CMake to run moc automatically when need
set(CMAKE_AUTOMOC ON)
#Create code from a list of Qt designer ui file
set(CMAKE_AUTOUIC ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG 	${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE 	${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG 	${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RLEASE 	${PROJECT_SOURCE_DIR}/bin)

find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Network CONFIG REQUIRED)
find_package(PythonLibs REQUIRED)

 add_subdirectory("./tcpSocket")
 add_subdirectory("./udpSocket")
 add_subdirectory("./PythonInterface")
 add_subdirectory("./Classify")
 add_subdirectory("./ImageRecognitionModule")
 add_subdirectory("./KeywordAnalsys")
 add_subdirectory("./GetIpAndMac")
 add_subdirectory("./LogService")
 add_subdirectory("./XmlReader")

# include_directories(${PYTHON_INCLUDE_DIRS})
# include_directories("./PythonInterface")
# include_directories("./Classify")
# include_directories("./udpSocket")
# include_directories("./tcpSocket")
# include_directories("./GetIpAndMac")
 include_directories("./XmlReader")

add_executable(Main_test main.cpp)
# target_link_libraries(Main_test -lpthread)
# target_link_libraries(Main_test PythonInterface_shared)
# target_link_libraries(Main_test Qt5::Widgets)
# target_link_libraries(Main_test Classify_shared)
# target_link_libraries(Main_test UdpSocket_shared)
# target_link_libraries(Main_test TcpSocket_shared)
# target_link_libraries(Main_test Qt5::Network)
# target_link_libraries(Main_test GetIpAndMac_shared)
 target_link_libraries(Main_test XmlReader_shared)