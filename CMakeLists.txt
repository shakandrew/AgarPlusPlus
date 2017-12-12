cmake_minimum_required(VERSION 3.9)

project(webrtc-server)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.65.1 REQUIRED COMPONENTS program_options system)
find_package(LibWebRTC REQUIRED)
find_package(Threads REQUIRED)

include(${LIBWEBRTC_USE_FILE})

include_directories("src/")

file(GLOB PROJECT_CONFIGS CMakeLists.txt
                          .gitattributes
                          .gitignore
                          .gitmodules)
if(Boost_FOUND)
    add_executable(webrtc-server
            src/webrtc_server.cpp
            src/webrtc_observers.h
    )
    set_target_properties(webrtc-server PROPERTIES
            OUTPUT_NAME webrtc-server
            RUNTIME_OUTPUT_DIRECTORY /${CMAKE_BINARY_DIR}/bin/
    )
    target_link_libraries(webrtc-server
            Boost::program_options
            Boost::system
            ${CMAKE_THREAD_LIBS_INIT}
            ${LIBWEBRTC_LIBRARIES}
            pthread
    )
endif()