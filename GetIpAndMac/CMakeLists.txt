AUX_SOURCE_DIRECTORY(. DIR_SRCS)

set(libGetIpAndMac ./GetIpAndMac.cpp)

include_directories("./")

add_library(GetIpAndMac_shared SHARED ${libGetIpAndMac})

if(UNIX)    # 用于UNIX平台开发
       
else()      # 用于win32平台开发
    target_link_libraries(GetIpAndMac_shared -lwsock32)
endif()


SET_TARGET_PROPERTIES (GetIpAndMac_shared PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES (GetIpAndMac_shared PROPERTIES VERSION 1.1 SOVERSION 1)