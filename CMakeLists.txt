# This project is licensed under the MIT license. For more information see the
# LICENSE file.

cmake_minimum_required(VERSION 2.8)

project(maddy)

enable_testing()

# ------------------------------------------------------------------------------

set(MADDY_CPP_VERSION 14)
add_definitions(-DCPP_VERSION=${MADDY_CPP_VERSION})

# ------------------------------------------------------------------------------

set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# ------------------------------------------------------------------------------

set(MADDY_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
file(GLOB_RECURSE MADDY_TESTS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/tests/maddy/*.cpp)

# ------------------------------------------------------------------------------

if (UNIX)
  set(
    CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -g -std=c++${MADDY_CPP_VERSION} -Wall -Wpedantic -Wextra -Wno-ignored-qualifiers -fno-rtti -fno-exceptions -fsanitize=address -fno-omit-frame-pointer"
  )
else()
  set(
    CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -g -std=c++${MADDY_CPP_VERSION} -fno-rtti -fno-exceptions -fsanitize=address -fno-omit-frame-pointer"
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

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/gtest/googlemock)
add_subdirectory(libs)

# ------------------------------------------------------------------------------

include_directories(
  ${LIBS_INCLUDE_DIRS}
  ${MADDY_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

# ------------------------------------------------------------------------------

add_executable(
  MaddyTests
    ${MADDY_TESTS_FILES}
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/main.cpp
)
target_link_libraries(MaddyTests gmock_main)
add_test(MaddyTests ${CMAKE_CURRENT_SOURCE_DIR}/build/MaddyTests)
