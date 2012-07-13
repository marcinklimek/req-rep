cmake_minimum_required(VERSION 2.8)

ADD_DEFINITIONS("-std=c++0x")

IF(NOT CMAKE_BUILD_TYPE)
      SET(CMAKE_BUILD_TYPE Debug CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

project(rep-zmq)
include_directories (.)

add_executable(rep-zmq server.cpp main.cpp )

target_link_libraries(rep-zmq stdc++ pthread boost_thread boost_system zmq)
