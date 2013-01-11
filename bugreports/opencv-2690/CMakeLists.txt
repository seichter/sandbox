cmake_minimum_required(VERSION 2.8)

project(farnebeck)

find_package(OpenCV REQUIRED)

add_executable(farneback farneback-test.cpp)

target_link_libraries(farneback ${OpenCV_LIBRARIES})
