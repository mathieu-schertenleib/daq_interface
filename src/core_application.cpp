#include "core_application.hpp"

#include "logging.hpp"

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#endif
#include "imgui.h"
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "implot.h"

#include <GLFW/glfw3.h>

namespace
{

void glfw_error_callback(int error, const char *description)
{
    log_error("GLFW error {}: {}", error, description);
}

} // namespace

constinit bool Core_application::s_active {false};

Core_application::Core_application()
{
    if (s_active)
    {
        throw std::runtime_error(
            "Only one application instance can be created at once");
    }

    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }

#if defined(__APPLE__)
    constexpr const char *glsl_version {"#version 150"};
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    constexpr const char *glsl_version {"#version 130"};
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    m_window = glfwCreateWindow(1280, 720, "DAQ", nullptr, nullptr);
    if (m_window == nullptr)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    if (!ImGui_ImplGlfw_InitForOpenGL(m_window, true))
    {
        ImGui::DestroyContext();

        glfwDestroyWindow(m_window);
        glfwTerminate();

        throw std::runtime_error(
            "Failed to initialize ImGui GLFW implementation for OpenGL");
    }

    if (!ImGui_ImplOpenGL3_Init(glsl_version))
    {
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(m_window);
        glfwTerminate();

        throw std::runtime_error(
            "Failed to initialize ImGui OpenGL implementation");
    }

    ImPlot::CreateContext();

    s_active = true;
}

Core_application::~Core_application() noexcept
{
    if (m_window == nullptr)
    {
        return;
    }

    s_active = false;

    ImPlot::DestroyContext();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_window);
    glfwTerminate();
}

Core_application::Core_application(Core_application &&rhs) noexcept
    : m_window {rhs.m_window}
{
    rhs.m_window = nullptr;
}

void Core_application::run()
{
    while (!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        update();

        ImGui::Render();

        int framebuffer_width;
        int framebuffer_height;
        glfwGetFramebufferSize(
            m_window, &framebuffer_width, &framebuffer_height);
        glViewport(0, 0, framebuffer_width, framebuffer_height);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_window);
    }
}
