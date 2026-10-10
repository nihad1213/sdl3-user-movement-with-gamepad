cmake_minimum_required(VERSION 3.31)

project(sdl3-knight-game C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Werror)

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

file(GLOB SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.c")

add_executable(game ${SOURCES})

target_include_directories(game PRIVATE "${CMAKE_SOURCE_DIR}/include")

target_link_libraries(game PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)