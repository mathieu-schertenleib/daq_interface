#include "application.hpp"

#include <fmt/color.h>

#include <cstdlib>
#include <iostream>

int main()
{
    try
    {
        fmt::print(fg(fmt::color::crimson) | fmt::emphasis::bold,
                   "Hello, {}!\n",
                   "world");
        fmt::print(fg(fmt::color::floral_white) | bg(fmt::color::slate_gray) |
                       fmt::emphasis::underline,
                   "Hello, {}!\n",
                   "мир");
        fmt::print(fg(fmt::color::steel_blue) | fmt::emphasis::italic,
                   "Hello, {}!\n",
                   "世界");

        Application app;
        app.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::cerr << "Unknown exception thrown\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
