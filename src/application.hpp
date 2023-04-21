#ifndef APPLICATION_HPP
#define APPLICATION_HPP

class Application
{
public:
    [[nodiscard]] Application();
    ~Application();

    Application(const Application &) = delete;
    Application &operator=(const Application &) = delete;

    Application(Application &&) = default;
    Application &operator=(Application &&) = default;

    void run();

private:
    void init();
    void shutdown();

    void update_ui();

    struct GLFWwindow *m_window {};
};

#endif // APPLICATION_HPP
