cmake_minimum_required(VERSION 3.19)

project(daq LANGUAGES CXX)

add_subdirectory(src)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)

add_subdirectory(external/fmt)
