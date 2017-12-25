# This project is licensed under the MIT license. For more information see the
# LICENSE file.

cmake_minimum_required(VERSION 2.8)

set(LIBS_INCLUDE_DIRS
# -- googletest / -mock --------------------------------------------------------
  ${CMAKE_CURRENT_SOURCE_DIR}/gtest/googletest/include
  ${CMAKE_CURRENT_SOURCE_DIR}/gtest/googlemock/include
# ------------------------------------------------------------------------------
PARENT_SCOPE)

set(LIBS_SRC_FILES
PARENT_SCOPE)
