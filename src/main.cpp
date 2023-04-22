#include "application.hpp"
#include "logging.hpp"

#include <cstdlib>
#include <stdexcept>

int main()
{
    try
    {
        log_info("This is an info message {}\n", 0);
        log_warning("This is a warning message {}\n", 1);
        log_error("This is an error message {}\n", 2);

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
