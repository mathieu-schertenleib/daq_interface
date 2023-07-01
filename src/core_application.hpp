#ifndef CORE_APPLICATION_HPP
#define CORE_APPLICATION_HPP

class Core_application
{
public:
    Core_application();

    virtual ~Core_application() noexcept;

    Core_application(Core_application &&rhs) noexcept;

    Core_application &operator=(Core_application &&rhs) noexcept = delete;
    Core_application(const Core_application &) = delete;
    Core_application &operator=(const Core_application &) = delete;

    void run();

private:
    virtual void update() = 0;

    static constinit bool s_active;
    struct GLFWwindow *m_window {nullptr};
};

#endif // CORE_APPLICATION_HPP
