cmake_minimum_required(VERSION 3.9)

project(webrtc-server)

if(EXISTS "${CMAKE_SOURCE_DIR}/LocalConfig.cmake")
  include(LocalConfig.cmake)
else()
  message(FATAL_ERROR "Local config is absent")
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

set (Boost_USE_MULTITHREADED ON)
set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_STATIC_RUNTIME OFF)
set (BOOST_ALL_DYN_LINK OFF)

include(CMake/CMakeFindExtensions.cmake)
include(CMake/CMakeHelpers.cmake)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")

find_package(Boost 1.65.1 REQUIRED COMPONENTS program_options system)
find_package(WebRTC REQUIRED)

include_directories(${Boost_INCLUDE_DIR})
include_directories(${WEBRTC_INCLUDE_DIR})

message(SOURSE " ${Boost_LIBRARIES}")
message(SOURSE " ${Boost_INCLUDE_DIRS}")

if(Boost_FOUND)
    add_executable(webrtc-server
            src/webrtc_server.cpp
            src/webrtc_observers.h
    )
    target_link_libraries(webrtc-server
            ${Boost_LIBRARIES}
            ${WEBRTC_LIBRARIES}
    )
endif()
