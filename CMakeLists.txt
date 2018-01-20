cmake_minimum_required(VERSION 3.10)

project(AgarPlusPlus)

set(CMAKE_CXX_STANDARD 14)

if(EXISTS "${CMAKE_SOURCE_DIR}/LocalConfig.cmake")
    include(LocalConfig.cmake)
else()
    message(FATAL_ERROR "Local config is absent")
endif()

set(CMAKE_MODULE_PATH
        ${CMAKE_MODULE_PATH}
        ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )

if(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND MSVC)
    foreach(CompilerFlag ${CompilerFlags})
        string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
        string(CONCAT ${CompilerFlag} "${${CompilerFlag}}" " /W3")
        message("${CompilerFlag} = ${${CompilerFlag}}")
    endforeach()
    add_definitions(-DNOMINMAX)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    foreach(CompilerFlag ${CompilerFlags})
        string(CONCAT ${CompilerFlag} "${${CompilerFlag}}" " -Wall")
        message("${CompilerFlag} = ${${CompilerFlag}}")
    endforeach()
endif()

find_package(Threads REQUIRED)

add_definitions(-DBOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE)
add_definitions(-DBOOST_ALL_NO_LIB)
set(BOOST_ROOT "")
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)
find_package(Boost 1.66.0 REQUIRED COMPONENTS program_options system unit_test_framework)

set(WEBRTC_ROOT_DIR "")
find_package(WebRTC REQUIRED)

set(RAPIDJSON_ROOT "")
find_package(RapidJSON REQUIRED)

set(SOURCE_FILES src/webrtc_observers.h src/webrtc_server.cpp)
if(Threads_FOUND AND Boost_FOUND AND WEBRTC_FOUND AND RAPIDJSON_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    include_directories(${WEBRTC_INCLUDE_DIRS})
    include_directories(${RAPIDJSON_INCLUDE_DIR})
    add_executable(agar ${SOURCE_FILES})
    target_link_libraries(agar ${CMAKE_THREAD_LIBS_INIT})
    target_link_libraries(agar ${Boost_LIBRARIES})
    target_link_libraries(agar ${WEBRTC_DEPENDENCIES})
    target_link_libraries(agar ${WEBRTC_LIBRARIES})
endif()
