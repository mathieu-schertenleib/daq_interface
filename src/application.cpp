#include "application.hpp"

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
    log_error("GLFW error {}: {}\n", error, description);
}

} // namespace

Application::Application()
{
    try
    {
        init();
    }
    catch (...)
    {
        shutdown();
        throw;
    }
}

Application::~Application()
{
    shutdown();
}

void Application::run()
{
    while (!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        update_ui();

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

void Application::init()
{
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
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImPlot::CreateContext();
}

void Application::shutdown()
{
    ImPlot::DestroyContext();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Application::update_ui()
{
#ifdef IMGUI_HAS_VIEWPORT
    auto *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->GetWorkPos());
    ImGui::SetNextWindowSize(viewport->GetWorkSize());
    ImGui::SetNextWindowViewport(viewport->ID);
#else
    ImGui::SetNextWindowPos({0.0f, 0.0f});
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
#endif
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    if (ImGui::Begin("Fullscreen window",
                     nullptr,
                     ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoBringToFrontOnFocus))
    {
        const float framerate {ImGui::GetIO().Framerate};
        ImGui::Text("%.3f ms/frame (%.1f fps)",
                    static_cast<double>(1000.0f / framerate),
                    static_cast<double>(framerate));
    }
    ImGui::End();
    ImGui::PopStyleVar();

    ImPlot::ShowDemoWindow();
}
