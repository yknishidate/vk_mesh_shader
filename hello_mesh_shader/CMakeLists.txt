cmake_minimum_required(VERSION 3.16)

set(TARGET_NAME "hello_mesh_shader")

file(GLOB_RECURSE shaders shader/*)
add_executable(${TARGET_NAME} main.cpp ${shaders})
source_group("Shader Files" FILES ${shaders})

target_link_libraries(${TARGET_NAME} PRIVATE 
    Reactive
)

target_include_directories(${TARGET_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/Reactive/source
)

target_compile_definitions(${TARGET_NAME} PRIVATE
    "SHADER_DIR=std::string{\"${CMAKE_CURRENT_SOURCE_DIR}/shader/\"}"
)
