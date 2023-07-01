cmake_minimum_required(VERSION 3.25)
project(test CXX)
set(CMAKE_CXX_STANDARD 23)

add_executable(test src/example.cpp ../include/string_interpolation.hpp)
#find_package(string_interpolation CONFIG REQUIRED)
#target_link_libraries(example string_interpolation::string_interpolation)
