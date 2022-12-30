cmake_minimum_required(VERSION 3.16)

project(vk_mesh_shader LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

set(REACTIVE_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(Reactive)

add_subdirectory(hello_mesh_shader)
