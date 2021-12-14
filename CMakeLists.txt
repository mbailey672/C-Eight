cmake_minimum_required(VERSION 3.10)

# set the project name
project(C-Eight)

find_package(SDL2 REQUIRED)
include_directories(C-Eight ${SDL2_INCLUDE_DIRS})
add_subdirectory(Chip8)
add_subdirectory(stack)

# add the executable
add_executable(C-Eight main.cpp)

# link sdl2
target_link_libraries(C-Eight PRIVATE ${SDL2_LIBRARIES})
target_link_libraries(C-Eight PRIVATE Chip8)
target_link_libraries(C-Eight PRIVATE stack)

target_include_directories(C-Eight PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_SOURCE_DIR}/Chip8"
                           "${PROJECT_SOURCE_DIR}/stack"
)

