# This project is licensed under the MIT license. For more information see the
# LICENSE file.

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        b796f7d44681514f58a683a3a71ff17c94edb0c1 # v1.13.0
)
FetchContent_MakeAvailable(googletest)

# ------------------------------------------------------------------------------

file(GLOB_RECURSE MADDY_TESTS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/maddy/*.cpp)

# ------------------------------------------------------------------------------

add_executable(
  MaddyTests
    ${MADDY_TESTS_FILES}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
target_include_directories(MaddyTests PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(MaddyTests maddy gmock_main)
add_test(NAME MaddyTests COMMAND MaddyTests)
