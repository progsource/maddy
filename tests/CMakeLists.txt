# This project is licensed under the MIT license. For more information see the
# LICENSE file.

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

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2
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
set_target_properties(MaddyTests PROPERTIES
  CMAKE_CXX_FLAGS
  "${CMAKE_CXX_FLAGS} -g -Wall -Wpedantic -Wextra -Wno-ignored-qualifiers -fno-rtti -fno-exceptions -fsanitize=address -fno-omit-frame-pointer"
)
add_test(NAME MaddyTests COMMAND MaddyTests)
