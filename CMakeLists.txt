# This project is licensed under the MIT license. For more information see the
# LICENSE file.

cmake_minimum_required(VERSION 3.25)

project(maddy)

# ------------------------------------------------------------------------------

set(MADDY_CPP_VERSION 14)
add_definitions(-DCPP_VERSION=${MADDY_CPP_VERSION})

# ------------------------------------------------------------------------------

set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# ------------------------------------------------------------------------------

option(MADDY_BUILD_WITH_TESTS "enable building tests" OFF)

if(${MADDY_BUILD_WITH_TESTS})
  enable_testing()
endif()

# ------------------------------------------------------------------------------

set(MADDY_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

# ------------------------------------------------------------------------------

if (UNIX)
  set(
    CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -g -std=c++${MADDY_CPP_VERSION} -Wall -Wpedantic -Wextra -Wno-ignored-qualifiers -fno-rtti -fno-exceptions -fsanitize=address -fno-omit-frame-pointer"
  )
endif()

# ------------------------------------------------------------------------------

if (UNIX AND NOT APPLE)
  execute_process(COMMAND ${CMAKE_CXX_COMPILER}
                  -fuse-ld=gold -Wl,--version
                  ERROR_QUIET OUTPUT_VARIABLE ld_version)
  if ("${ld_version}" MATCHES "GNU gold")
    message(STATUS "Found Gold linker, use faster linker")
    set(CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")
    set(CMAKE_SHARED_LINKER_FLAGS
        "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=gold ")
  endif()
endif()

# ------------------------------------------------------------------------------

add_library(maddy INTERFACE)
target_include_directories(maddy INTERFACE
  ${MADDY_INCLUDE_DIR}
)

# ------------------------------------------------------------------------------

if(${MADDY_BUILD_WITH_TESTS})
  add_subdirectory(tests)
endif()
