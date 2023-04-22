set(IMGUI_CONFIG "${CMAKE_SOURCE_DIR}/src/imgui_config.h")
add_compile_definitions(
        $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>>:NDEBUG>
        IMGUI_USER_CONFIG="${IMGUI_CONFIG}"
        _CRT_SECURE_NO_WARNINGS)

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported_result OUTPUT ipo_supported_output)
if (ipo_supported_result)
    set_property(GLOBAL PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
    set_property(GLOBAL PROPERTY INTERPROCEDURAL_OPTIMIZATION_MIN_SIZE_REL TRUE)
else ()
    message(WARNING "IPO is not supported: ${ipo_supported_output}")
endif ()


set(IMGUI_DIRECTORY ${CMAKE_SOURCE_DIR}/external/imgui)
add_library(imgui STATIC
        ${IMGUI_DIRECTORY}/imgui.cpp ${IMGUI_DIRECTORY}/imgui.h
        ${IMGUI_DIRECTORY}/imgui_internal.h
        ${IMGUI_DIRECTORY}/imgui_draw.cpp
        ${IMGUI_DIRECTORY}/imgui_tables.cpp
        ${IMGUI_DIRECTORY}/imgui_widgets.cpp
        ${IMGUI_DIRECTORY}/imgui_demo.cpp
        ${IMGUI_DIRECTORY}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIRECTORY}/backends/imgui_impl_opengl3.cpp)
target_include_directories(imgui PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${IMGUI_DIRECTORY}
        ${IMGUI_DIRECTORY}/backends)
target_link_libraries(imgui PRIVATE OpenGL::GL glfw)


set(IMPLOT_DIRECTORY ${CMAKE_SOURCE_DIR}/external/implot)
add_library(implot STATIC
        ${IMPLOT_DIRECTORY}/implot.cpp ${IMPLOT_DIRECTORY}/implot.h
        ${IMPLOT_DIRECTORY}/implot_internal.h
        ${IMPLOT_DIRECTORY}/implot_items.cpp
        ${IMPLOT_DIRECTORY}/implot_demo.cpp)
target_include_directories(implot PRIVATE
        ${IMPLOT_DIRECTORY}
        ${IMGUI_DIRECTORY})
target_link_libraries(implot PRIVATE imgui)


add_executable(daq_interface
        main.cpp
        application.cpp application.hpp
        port.hpp
        logging.hpp)

if (WIN32)
    target_sources(daq_interface PRIVATE port_win.cpp)
else ()
    target_sources(daq_interface PRIVATE port_unix.cpp)
endif ()

target_include_directories(daq_interface PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${IMGUI_DIRECTORY}
        ${IMGUI_DIRECTORY}/backends
        ${IMPLOT_DIRECTORY})
target_compile_features(daq_interface PRIVATE cxx_std_20)
target_link_libraries(daq_interface PRIVATE OpenGL::GL glfw fmt::fmt imgui implot)

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
    target_compile_options(daq_interface PRIVATE ${CLANG_OPTIONS})
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(daq_interface PRIVATE ${GCC_OPTIONS})
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(daq_interface PRIVATE /W4 /Zc:__cplusplus)
else ()
    message(WARNING "No compile options set for compiler '${CMAKE_CXX_COMPILER_ID}'")
endif ()
