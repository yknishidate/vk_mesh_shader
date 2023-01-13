cmake_minimum_required(VERSION 3.16)
include($ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

project(vk_mesh_shader LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

# find_package(meshoptimizer CONFIG REQUIRED)
# target_link_libraries(vk_mesh_shader PUBLIC 
#     meshoptimizer::meshoptimizer)

set(REACTIVE_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(Reactive)

add_subdirectory(hello_mesh_shader)
add_subdirectory(gpu_subdivision)
add_subdirectory(mesh_inline_rt)
