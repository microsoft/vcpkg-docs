cmake_minimum_required(VERSION 3.18)

project(demo CXX)

add_executable(main main.cpp)

find_package(beicode CONFIG REQUIRED)
find_package(beison CONFIG REQUIRED)
target_link_libraries(main PRIVATE beicode::beicode beison::beison)
