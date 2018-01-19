cmake_minimum_required(VERSION 3.7)

project(Backy LANGUAGES C CXX)

add_compile_options(-std=c++11 -Wall -g)

set(CMAKE_PREFIX_PATH /opt/Qt/5.10.0/gcc_64/lib/cmake)
find_package(Qt5Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/../include/*.h")
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../src/*.cpp")

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

include_directories(${CMAKE_CURRENT_BINARY_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/../include")
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
