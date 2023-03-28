#include "application.hpp"

#include <GLFW/glfw3.h>

#include <sstream>

namespace
{

[[noreturn]] void glfw_error_callback(int error, const char *description)
{
    std::ostringstream oss;
    oss << "GLFW error " << error << ": " << description << '\n';
    throw std::runtime_error(oss.str());
}

} // namespace

Application::Application()
{
    glfwSetErrorCallback(glfw_error_callback);
    glfwInit();

    m_window = glfwCreateWindow(640, 360, "DAQ", nullptr, nullptr);
}

Application::~Application()
{
    glfwDestroyWindow(m_window);

    glfwTerminate();
}

void Application::run()
{
    while (!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();
    }
}
