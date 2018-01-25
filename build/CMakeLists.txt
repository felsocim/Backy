cmake_minimum_required(VERSION 3.5)

project(Backy LANGUAGES C CXX)

add_compile_options(-std=c++11 -Wall -g)

set(CMAKE_PREFIX_PATH /home/marek/Qt/5.10.0/gcc_64/lib/cmake)
find_package(Qt5Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/../include/*")
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../src/*")

set(CMAKE_AUTORCC ON)
file(GLOB RESOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../res/*.qrc")

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${RESOURCES})

include_directories(${CMAKE_CURRENT_BINARY_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/../include")
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
