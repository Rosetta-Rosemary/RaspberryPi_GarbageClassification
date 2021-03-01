AUX_SOURCE_DIRECTORY(. DIR_SRCS)

set(libUdpSocket ./udpSocket.cpp)

include_directories("./")

find_package(Qt5Network CONFIG REQUIRED)
add_library(UdpSocket_shared SHARED ${libUdpSocket})

target_link_libraries(UdpSocket_shared Qt5::Network)
target_link_libraries(UdpSocket_shared -lpthread)


SET_TARGET_PROPERTIES (UdpSocket_shared PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES (UdpSocket_shared PROPERTIES VERSION 1.1 SOVERSION 1)