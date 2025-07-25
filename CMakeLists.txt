# This project is licensed under the MIT license. For more information see the
# LICENSE file.

cmake_minimum_required(VERSION 3.25)

project(maddy VERSION 1.6.0) # MADDY_VERSION_LINE_REPLACEMENT

# ------------------------------------------------------------------------------

include(GNUInstallDirs)
set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# ------------------------------------------------------------------------------

option(MADDY_BUILD_WITH_TESTS "enable building tests - does not work with zip download" OFF)

if(${MADDY_BUILD_WITH_TESTS})
  enable_testing()
endif()

option(MADDY_BUILD_WITH_BENCH "enable benchmarks" OFF)
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
  $<BUILD_INTERFACE:${MADDY_INCLUDE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# ------------------------------------------------------------------------------

if(${MADDY_BUILD_WITH_TESTS})
  add_subdirectory(tests)
endif()

if(${MADDY_BUILD_WITH_BENCH})
  add_subdirectory(bench)
endif()

# ------------------------------------------------------------------------------

if(${MADDY_CREATE_PACKAGE})
  set(MADDY_PACKAGE_FILES include/ CMakeLists.txt LICENSE AUTHORS)
  add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-src.zip
    COMMAND ${CMAKE_COMMAND} -E tar c ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-src.zip --format=zip -- ${MADDY_PACKAGE_FILES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ${MADDY_PACKAGE_FILES})
  add_custom_target(${PROJECT_NAME}_package DEPENDS ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-src.zip)
endif()


include(CMakePackageConfigHelpers)

install(DIRECTORY include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMinorVersion
)
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
