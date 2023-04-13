cmake_minimum_required(VERSION 3.10)

project(server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -o2 -Wall -")


include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(server  src/Acceptor.cpp src/Buffer.cpp src/Channel.cpp src/Connection.cpp src/Epoll.cpp src/EventLoop.cpp src/InetAddress.cpp src/Server.cpp src/Socket.cpp  src/util.cpp)
# add_executable(server_cmake)
# file(GLOB sources CMAKE_CONFIGURE_DEPENDS *.cpp *.h)
# target_sources(server_cmake PUBLIC ${sources})