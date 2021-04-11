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

 add_subdirectory("./LogService")
 add_subdirectory("./XmlReader")
 add_subdirectory("./Classify")
 add_subdirectory("./KeywordAnalsys")
 add_subdirectory("./Network")
 add_subdirectory("./PiServer")
 add_subdirectory("./TerminalModule")