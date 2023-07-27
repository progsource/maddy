# This project is licensed under the MIT license. For more information see the
# LICENSE file.

cmake_minimum_required(VERSION 3.25)

project(maddy)

# ------------------------------------------------------------------------------

set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# ------------------------------------------------------------------------------

option(MADDY_BUILD_WITH_TESTS "enable building tests - does not work with zip download" OFF)

if(${MADDY_BUILD_WITH_TESTS})
  enable_testing()
endif()

option(MADDY_CREATE_PACKAGE "create a package for a version release" OFF)

# ------------------------------------------------------------------------------

set(MADDY_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

# ------------------------------------------------------------------------------

if(NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(${CMAKE_CXX_STANDARD} LESS 14)
  message(FATAL_ERROR "maddy requires >=C++14")
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

# ------------------------------------------------------------------------------

if(${MADDY_CREATE_PACKAGE})
  set(MADDY_PACKAGE_FILES include/ CMakeLists.txt LICENSE)
  add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-src.zip
    COMMAND ${CMAKE_COMMAND} -E tar c ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-src.zip --format=zip -- ${MADDY_PACKAGE_FILES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ${MADDY_PACKAGE_FILES})
  add_custom_target(${PROJECT_NAME}_package DEPENDS ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-src.zip)
endif()
