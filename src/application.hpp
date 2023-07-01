#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "core_application.hpp"

#include <chrono>
#include <condition_variable>
#include <thread>

class Daq_application : public Core_application
{
private:
    void update() override;

    void start_timer();
    void stop_timer();

    std::jthread m_timer_thread;
    std::condition_variable m_timer_cv;
};

#endif // APPLICATION_HPP
