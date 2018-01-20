########################################################################
# CMake module for finding WEBRTC
#
# The best way to build WebRTC is with the forked `webrtcbuilds` project here:
# https://github.com/sourcey/webrtcbuilds
#
# The following variables will be defined:
#
#  WEBRTC_FOUND
#  WEBRTC_INCLUDE_DIR
#  WEBRTC_LIBRARIES
#  WEBRTC_DEPENDENCIES
#

# unset(WEBRTC_INCLUDE_DIR)
# unset(WEBRTC_INCLUDE_DIR CACHE)
# unset(WEBRTC_LIBRARY)
# unset(WEBRTC_LIBRARY CACHE)
# unset(WEBRTC_LIBRARIES)
# unset(WEBRTC_LIBRARIES CACHE)
# unset(WEBRTC_LIBRARIES_DEBUG)
# unset(WEBRTC_LIBRARIES_DEBUG CACHE)
# unset(WEBRTC_LIBRARIES_RELEASE)
# unset(WEBRTC_LIBRARIES_RELEASE CACHE)

include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/CMakeFindExtensions.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/CMakeHelpers.cmake)

# Set required variables
# set(WEBRTC_ROOT_DIR "")
# set(WEBRTC_BUILD_DIR_SUFFIX_DEBUG "out/Debug")
# set(WEBRTC_BUILD_DIR_SUFFIX_RELEASE "out/Release")

# ----------------------------------------------------------------------
# Find WEBRTC include path
# ----------------------------------------------------------------------
find_path(WEBRTC_INCLUDE_DIR
        NAMES
        "webrtc/typedefs.h"
        PATHS
        ${WEBRTC_ROOT_DIR}/include
        )

# ----------------------------------------------------------------------
# Find WEBRTC libraries
# ----------------------------------------------------------------------
if(WEBRTC_INCLUDE_DIR)
  find_existing_directory(debug_dir
          ${WEBRTC_ROOT_DIR}/lib/x64/Debug
          ${WEBRTC_ROOT_DIR}/out/x64/Debug
          ${WEBRTC_ROOT_DIR}/out/Debug_x64
          ${WEBRTC_ROOT_DIR}/out/Debug-x64
          ${WEBRTC_ROOT_DIR}/out/Debug)

  find_existing_directory(release_dir
          ${WEBRTC_ROOT_DIR}/lib/x64/Release
          ${WEBRTC_ROOT_DIR}/out/x64/Release
          ${WEBRTC_ROOT_DIR}/out/Release_x64
          ${WEBRTC_ROOT_DIR}/out/Release-x64
          ${WEBRTC_ROOT_DIR}/out/Release)

  # Attempt to find the monolithic library built with `webrtcbuilds`
  find_library_extended(WEBRTC
          NAMES webrtc webrtc_full libwebrtc_full
          PATHS_DEBUG ${debug_dir}
          PATHS_RELEASE ${release_dir}
          )

  # Add required system libraries
  if(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND MSVC)
    add_definitions(-DWEBRTC_WIN)
    set(WEBRTC_DEPENDENCIES Secur32.lib Winmm.lib msdmo.lib dmoguids.lib wmcodecdspuuid.lib)
  elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_definitions(-DWEBRTC_POSIX)


    # For ABI compatability between precompiled WebRTC libraries using clang and new GCC versions
    add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)
    set(WEBRTC_DEPENDENCIES -lrt -lX11 -lGLU -ldl) # -lGL

    # Enable libstdc++ debugging if you build WebRTC with `enable_iterator_debugging=true`
    #set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG=1")
  endif()

  # Add vendor include directories
  if(WEBRTC_INCLUDE_DIR)
    set(WEBRTC_INCLUDE_DIRS ${WEBRTC_INCLUDE_DIR})
  endif()

  # workaround for fixing error WEBRTC_LIBRARY-NOTFOUND
  set(WEBRTC_LIBRARY_RELEASE ${WEBRTC_LIBRARIES_RELEASE})
  set(WEBRTC_LIBRARY_DEBUG ${WEBRTC_LIBRARIES_DEBUG})
  include(${CMAKE_ROOT}/Modules/SelectLibraryConfigurations.cmake)
  select_library_configurations(WEBRTC)

  # message("WEBRTC_LIBRARIES_DEBUG: ${WEBRTC_LIBRARIES_DEBUG}")
  # message("WEBRTC_LIBRARIES_RELEASE: ${WEBRTC_LIBRARIES_RELEASE}")
  # message("WEBRTC_LIBRARIES: ${WEBRTC_LIBRARIES}")
endif()

# ----------------------------------------------------------------------
# Display status
# ----------------------------------------------------------------------
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(WEBRTC DEFAULT_MSG WEBRTC_LIBRARIES WEBRTC_INCLUDE_DIR)

#print_module_variables(WEBRTC)

# HACK: WEBRTC_LIBRARIES and WEBRTC_DEPENDENCIES not propagating to parent scope
# while the WEBRTC_DEBUG_LIBRARY and WEBRTC_RELEASE_LIBRARY vars are.
# Setting PARENT_SCOPE fixes this issue for now.
set(WEBRTC_LIBRARIES ${WEBRTC_LIBRARIES} CACHE INTERNAL "")
set(WEBRTC_DEPENDENCIES ${WEBRTC_DEPENDENCIES} CACHE INTERNAL "")
set(WEBRTC_INCLUDE_DIRS ${WEBRTC_INCLUDE_DIRS} CACHE INTERNAL "")
set(WEBRTC_FOUND ${WEBRTC_FOUND} CACHE INTERNAL "")


mark_as_advanced(WEBRTC_LIBRARIES WEBRTC_INCLUDE_DIR
        WEBRTC_LIBRARIES_DEBUG WEBRTC_LIBRARIES_RELEASE
        WEBRTC_BUILD_DIR_SUFFIX_DEBUG WEBRTC_BUILD_DIR_SUFFIX_RELEASE
        WEBRTC_DEPENDENCIES)
