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
  nanobench
  GIT_REPOSITORY https://github.com/martinus/nanobench.git
  GIT_TAG v4.3.11
  GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(nanobench)

# ------------------------------------------------------------------------------

file(GLOB_RECURSE MADDY_BENCHMARK_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

# ------------------------------------------------------------------------------

add_executable(
  maddy_benchmark
    ${MADDY_BENCHMARK_FILES}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
target_include_directories(maddy_benchmark PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(maddy_benchmark maddy nanobench::nanobench)
set_target_properties(maddy_benchmark PROPERTIES
  CMAKE_CXX_FLAGS
  "${CMAKE_CXX_FLAGS} -O2 -Wall -Wno-ignored-qualifiers -Wpedantic -Wextra -Wno-deprecated -fno-exceptions -fno-rtti"
)
