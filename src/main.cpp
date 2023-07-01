#include "application.hpp"
#include "logging.hpp"

#include <cstdlib>
#include <stdexcept>

int main()
{
    try
    {
        Daq_application app;
        app.run();
        return EXIT_SUCCESS;
    }
    catch (const std::exception &e)
    {
        log_error("Exception thrown: {}", e.what());
        return EXIT_FAILURE;
    }
    catch (...)
    {
        log_error("Unknown exception thrown");
        return EXIT_FAILURE;
    }
}
