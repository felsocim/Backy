cmake_minimum_required(VERSION 3.5)

project(Backy LANGUAGES C CXX)

add_compile_options(-std=c++11 -Wall -g -lm)

set(CMAKE_PREFIX_PATH /home/marek/Qt/5.10.0/gcc_64/lib/cmake)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5LinguistTools REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

file(GLOB HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/../include/*.h")
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../src/*.cpp")
file(GLOB INTERFACES "${CMAKE_CURRENT_SOURCE_DIR}/../src/*.ui")
file(GLOB RESOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../res/*.qrc")
file(GLOB TRANSLATIONS "${CMAKE_CURRENT_SOURCE_DIR}/../translate/*.ts")

set(TRANSLATABLE ${HEADERS} ${SOURCES} ${INTERFACES})

foreach(translation fr sk)
  execute_process(COMMAND /home/marek/Qt/5.10.0/gcc_64/bin/lupdate -silent ${TRANSLATABLE} -ts "${CMAKE_CURRENT_SOURCE_DIR}/../translate/${PROJECT_NAME}_${translation}.ts")
endforeach(translation)

qt5_add_translation(BINARY_TRANSLATIONS ${TRANSLATIONS})

add_custom_target(translate DEPENDS ${BINARY_TRANSLATIONS})

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${INTERFACES} ${RESOURCES})

include_directories(${CMAKE_CURRENT_BINARY_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/../include")
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)

set_directory_properties(PROPERTIES CLEAN_NO_CUSTOM true)
