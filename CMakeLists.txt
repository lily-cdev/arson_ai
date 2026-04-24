cmake_minimum_required(VERSION 3.16)
project(arson LANGUAGES C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SDL_SHARED OFF)
set(SDL_STATIC ON)
add_subdirectory(SDL3)

add_executable(arson
    source/main.c
    source/generation.c
    source/tools.c
    source/town.c
    source/preloaders.c
    source/subgeneration.c
    source/tiles.c
    source/movement.c
    source/input.c
    source/noise.c
    source/collision.c
    source/sensors.c
    source/scoring.c
)

target_include_directories(arson PRIVATE include)

target_link_libraries(arson PRIVATE SDL3::SDL3)