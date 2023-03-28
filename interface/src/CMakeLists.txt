find_package(OpenGL REQUIRED)

set(IMGUI_DIRECTORY ../external/imgui)
set(IMGUI_FILES
        ${IMGUI_DIRECTORY}/imgui.cpp ${IMGUI_DIRECTORY}/imgui.h
        ${IMGUI_DIRECTORY}/imgui_internal.h
        ${IMGUI_DIRECTORY}/imgui_draw.cpp
        ${IMGUI_DIRECTORY}/imgui_tables.cpp
        ${IMGUI_DIRECTORY}/imgui_widgets.cpp
        ${IMGUI_DIRECTORY}/imgui_demo.cpp
        ${IMGUI_DIRECTORY}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIRECTORY}/backends/imgui_impl_opengl3.cpp)
set(IMPLOT_DIRECTORY ../external/implot)
set(IMPLOT_FILES
        ${IMPLOT_DIRECTORY}/implot.cpp ${IMPLOT_DIRECTORY}/implot.h
        ${IMPLOT_DIRECTORY}/implot_internal.h
        ${IMPLOT_DIRECTORY}/implot_items.cpp
        ${IMPLOT_DIRECTORY}/implot_demo.cpp)
add_executable(daq
        main.cpp
        application.cpp application.hpp
        ${IMGUI_FILES}
        ${IMPLOT_FILES})

target_include_directories(daq PRIVATE
        .
        ${IMGUI_DIRECTORY}
        ${IMGUI_DIRECTORY}/backends
        ${IMPLOT_DIRECTORY})

target_compile_features(daq PRIVATE cxx_std_20)

target_link_libraries(daq OpenGL::GL glfw)

set(CLANG_OPTIONS
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
    set_source_files_properties(
            ${IMGUI_FILES} ${IMPLOT_FILES}
            PROPERTIES
            COMPILE_FLAGS
            "-Wno-old-style-cast -Wno-conversion -Wno-sign-conversion -Wno-double-promotion")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(daq PRIVATE ${GCC_OPTIONS})
    set_source_files_properties(
            ${IMGUI_FILES} ${IMPLOT_FILES}
            PROPERTIES
            COMPILE_FLAGS
            "-Wno-old-style-cast -Wno-conversion -Wno-sign-conversion -Wno-double-promotion")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(daq PRIVATE /W4 /Zc:__cplusplus)
else ()
    message(WARNING "No compile options set for compiler '${CMAKE_CXX_COMPILER_ID}'")
endif ()

target_compile_definitions(daq PRIVATE
        IMGUI_USER_CONFIG="imgui_config.h"
        $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>>:NDEBUG>
        _CRT_SECURE_NO_WARNINGS)
