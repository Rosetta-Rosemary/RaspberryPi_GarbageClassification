AUX_SOURCE_DIRECTORY(. DIR_SRCS)

set(libTcpSocket ./tcpSocket.cpp)

include_directories("./")

find_package(Qt5Network CONFIG REQUIRED)
add_library(TcpSocket_shared SHARED ${libTcpSocket})
#add_library( PythonInterface_static STATIC ${libPythonInterface})
target_link_libraries(TcpSocket_shared Qt5::Network)

# 3、cmake在构建一个新的target时，会尝试清理掉其他使用这个名字的库，
# 因此，在构建libhello.a时，就会清理掉libhello.so
SET_TARGET_PROPERTIES (TcpSocket_shared PROPERTIES CLEAN_DIRECT_OUTPUT 1)
# 4、按照规则，动态库是应该包含一个版本号的，
# VERSION指代动态库版本，SOVERSION指代API版本。
SET_TARGET_PROPERTIES (TcpSocket_shared PROPERTIES VERSION 1.1 SOVERSION 1)


#SET_TARGET_PROPERTIES (PythonInterface_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
#SET_TARGET_PROPERTIES (PythonInterface_static PROPERTIES VERSION 1.1 SOVERSION 1)

# 5、若将libhello.a, libhello.so.x以及hello.h安装到系统目录，才能真正让其他人开发使用，
# 本例中，将hello的共享库安装到<prefix>/lib目录；
# 将hello.h安装<prefix>/include/hello目录。
#INSTALL (TARGETS hello hello_shared LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
#INSTALL (TARGETS hello hello_static LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
#INSTALL (FILES hello.h DESTINATION include/hello)