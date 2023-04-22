#include "port.hpp"

#include "logging.hpp"

#include <windows.h>

namespace
{

HANDLE g_handle {INVALID_HANDLE_VALUE};

}

bool open_port(const char *name)
{
    g_handle = CreateFile(name,
                          GENERIC_READ | GENERIC_WRITE,
                          0,
                          nullptr,
                          OPEN_EXISTING,
                          FILE_FLAG_OVERLAPPED,
                          nullptr);
    if (g_handle == INVALID_HANDLE_VALUE)
    {
        log_error("Failed to open port \"{}\"", name);
        return false;
    }

    return true;
}

void close_port()
{
    log_error("UNIMPLEMENTED\n");
}
