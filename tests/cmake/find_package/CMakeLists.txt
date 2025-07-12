# This project is licensed under the MIT license. For more information see the
# LICENSE file.
cmake_minimum_required(VERSION 3.25)

find_package(maddy REQUIRED)

add_executable(maddy_find_package_example main.cpp)
target_link_libraries(maddy_find_package_example PRIVATE maddy::maddy)
