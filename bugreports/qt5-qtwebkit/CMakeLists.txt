cmake_minimum_required(VERSION 2.8.8)
project(qwebkitimage)

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Declarative)
find_package(Qt5WebKit)
find_package(Qt5WebKitWidgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions(${Qt5Widgets_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

set(other_libs)
list(APPEND other_libs Qt5::WinMain)

add_executable(qwebkitimage MACOSX_BUNDLE WIN32
	main.cpp
	webwidget.h
	webwidget.cpp
	index.html
	qt-logo-small.png
	)

get_target_property(APP_LOCATION qwebkitimage LOCATION)
get_filename_component(APP_LOCATION_PATH ${APP_LOCATION} PATH)

add_custom_command(TARGET qwebkitimage PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E make_directory ${APP_LOCATION_PATH}/../share
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/index.html ${APP_LOCATION_PATH}/../share/index.html
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/qt-logo-small.png ${APP_LOCATION_PATH}/../share/qt-logo-small.png
	)

qt5_use_modules(qwebkitimage Core Gui Widgets WebKit WebKitWidgets)

target_link_libraries(qwebkitimage ${other_libs})

