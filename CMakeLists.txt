cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
cmake_policy(VERSION 3.8)
project(IGT-Project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Debug")

add_subdirectory("IGT - Project/SDL3")
add_subdirectory("IGT - Project/SDL_ttf")
add_subdirectory("IGT - Project/SDL_image")
add_subdirectory("IGT - Project/SDL_mixer")

add_subdirectory("IGT - Project")

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

