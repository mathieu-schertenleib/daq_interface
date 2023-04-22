#include "application.hpp"
#include "logging.hpp"

#include <cstdlib>
#include <stdexcept>

int main()
{
    try
    {
        Application app;
        app.run();
    }
    catch (const std::exception &e)
    {
        log_error("Exception thrown: {}\n", e.what());
        return EXIT_FAILURE;
    }
    catch (...)
    {
        log_error("Unknown exception thrown\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
