cmake_minimum_required(VERSION 3.0)

project(cmakeosg)

find_package(OpenGL REQUIRED)
find_package(OpenSceneGraph REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${OPENSCENEGRAPH_INCLUDE_DIRS})

add_executable(osgapp osgapp.cpp)

target_link_libraries(osgapp ${OPENSCENEGRAPH_LIBRARIES} ${OPENGL_LIBRARIES})
