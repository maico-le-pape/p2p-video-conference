cmake_minimum_required(VERSION 2.6)

project(p2p-video-conference)

if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "-std=gnu++11")
  set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -pedantic")
endif()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

find_package(Boost 1.40 REQUIRED thread date_time)
find_package(Vpx REQUIRED)
find_package(SDL REQUIRED)

include (UseDoxygen)

link_directories(${Boost_LIBRARY_DIRS})
include_directories(src ${Boost_INCLUDE_DIRS} ${VPX_INCLUDE_DIRS} ${SDL_INCLUDE_DIR})

add_library(epyx STATIC src/core/actor-manager.cpp
			src/core/actor.cpp
			src/core/exception.cpp
			src/core/log-worker.cpp
			src/core/log.cpp
			src/core/string.cpp
			src/core/thread.cpp
			src/core/timeout.cpp
			src/net/ipaddress.cpp
			src/net/netif.cpp
			src/net/netselect.cpp
			src/net/netselectreader.cpp
			src/net/netselectsocket.cpp
			src/net/server.cpp
			src/net/sockaddress.cpp
			src/net/tcpserver.cpp
			src/net/tcpsocket.cpp
			src/net/udpserver.cpp
			src/net/udpsocket.cpp
			src/net/socket.cpp
			src/net/uri.cpp
			src/parser/gttpacket.cpp
			src/parser/gttparser.cpp
			src/parser/httpparser.cpp
			src/parser/lineparser.cpp)
			
target_link_libraries(epyx ${Boost_LIBRARIES})

add_executable(video_conference src/receiver.cpp 
				src/packets/rttreplypacket.cpp
				src/packets/rttrequestpacket.cpp
				src/rttmanager.cpp
				src/main.cpp
				src/user.cpp
				src/videoconferencep2p.cpp)

target_link_libraries(video_conference epyx)