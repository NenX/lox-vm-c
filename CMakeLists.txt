cmake_minimum_required(VERSION 3.10)

project(clox)

file(GLOB_RECURSE SOURCES src/*.c)
add_executable(clox ${SOURCES})

target_compile_options(clox PRIVATE -Wimplicit-int)
target_include_directories(clox PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
