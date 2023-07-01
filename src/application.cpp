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

#include "implot.h"

void Daq_application::update()
{
    auto &io = ImGui::GetIO();

#ifdef IMGUI_HAS_VIEWPORT
    auto *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->GetWorkPos());
    ImGui::SetNextWindowSize(viewport->GetWorkSize());
    ImGui::SetNextWindowViewport(viewport->ID);
#else
    ImGui::SetNextWindowPos({0.0f, 0.0f});
    ImGui::SetNextWindowSize(io.DisplaySize);
#endif
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    if (ImGui::Begin("Fullscreen window",
                     nullptr,
                     ImGuiWindowFlags_NoDecoration |
                         ImGuiWindowFlags_NoBringToFrontOnFocus))
    {
        ImGui::Text("%.3f ms/frame (%.1f fps)",
                    static_cast<double>(1000.0f / io.Framerate),
                    static_cast<double>(io.Framerate));
        if (ImGui::Button("Start timer"))
        {
            start_timer();
        }
        if (ImGui::Button("Stop timer"))
        {
            stop_timer();
        }
    }
    ImGui::End();
    ImGui::PopStyleVar();

    ImPlot::ShowDemoWindow();
}

void Daq_application::start_timer()
{
    m_timer_thread = std::jthread(
        [](const std::stop_token &stoken)
        {
            unsigned int ping_count {};
            auto next_wakeup = std::chrono::system_clock::now();
            while (!stoken.stop_requested())
            {
                log_info("Ping from timer thread: {}", ping_count);
                ++ping_count;
                next_wakeup += std::chrono::milliseconds(500);
                std::this_thread::sleep_until(next_wakeup);
            }

            log_info("Timer thread requested to stop");
        });
}

void Daq_application::stop_timer()
{
    m_timer_thread.request_stop();
}
