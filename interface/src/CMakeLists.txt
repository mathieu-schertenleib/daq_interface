add_library(imgui STATIC
        ../external/imgui/imgui.cpp
        ../external/imgui/imgui_draw.cpp
        ../external/imgui/imgui_tables.cpp
        ../external/imgui/imgui_widgets.cpp
        ../external/imgui/backends/imgui_impl_glfw.cpp
        ../external/imgui/backends/imgui_impl_opengl3.cpp)

target_include_directories(imgui PRIVATE
        ../external/imgui
        ../external/imgui/backends
        ../external/glfw/include)


find_package(OpenGL REQUIRED)


add_executable(daq
        main.cpp
        application.cpp application.hpp)

target_include_directories(daq PRIVATE
        ${CMAKE_SOURCE_DIR}/external/imgui
        ${CMAKE_SOURCE_DIR}/external/imgui/backends
        ${CMAKE_SOURCE_DIR}/external/stb)

target_compile_features(daq PRIVATE cxx_std_20)

target_link_libraries(daq OpenGL::GL glfw imgui)

set(CLANG_OPTIONS
        #-march=native
        #-ffast-math
        -Wall
        -Wextra
        -Wshadow
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wcast-align
        -Wunused
        -Woverloaded-virtual
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Wnull-dereference
        -Wdouble-promotion)

set(GCC_OPTIONS
        ${CLANG_OPTIONS}
        -Wmisleading-indentation
        -Wduplicated-cond
        -Wduplicated-branches
        -Wlogical-op)

if (CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    target_compile_options(daq PRIVATE ${CLANG_OPTIONS})
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(daq PRIVATE ${GCC_OPTIONS})
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(daq PRIVATE /W4 /Zc:__cplusplus)
else ()
    message(WARNING "No compile options set for compiler '${CMAKE_CXX_COMPILER_ID}'")
endif ()

target_compile_definitions(daq PRIVATE
        $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>>:NDEBUG>
        _CRT_SECURE_NO_WARNINGS)
