cmake_minimum_required(VERSION 3.16)

include($ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

project(vk_mesh_shader LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

set(REACTIVE_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(Reactive)

file(GLOB_RECURSE sources source/*.cpp)
file(GLOB_RECURSE headers source/*.hpp)
file(GLOB_RECURSE shaders shader/*)
add_executable(${PROJECT_NAME} ${sources} ${headers} ${shaders})
source_group("Shader Files" FILES ${shaders})

target_link_libraries(${PROJECT_NAME} PUBLIC 
    Reactive
)

target_include_directories(${PROJECT_NAME} PUBLIC
    Reactive/source
    ${PROJECT_SOURCE_DIR}/source
)
