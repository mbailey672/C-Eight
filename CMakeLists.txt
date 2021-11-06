cmake_minimum_required(VERSION 3.10)

# set the project name
project(C-Eight)

# set build type
set(CMAKE_BUILD_TYPE Debug)

# bring headers into project
include_directories(include)

# get library files
file(GLOB SOURCES "src/*.cpp")

# find sdl2
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)
find_package(SDL2 REQUIRED)

# add the executable
add_executable(C-Eight main.cpp)
add_library(supportingClasses ${SOURCES})

# link sdl2
target_link_libraries(C-Eight SDL2::Main)
target_link_libraries(C-Eight supportingClasses)

