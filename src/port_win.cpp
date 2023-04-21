#include "port.hpp"

#include <windows.h>

#include <iostream>

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
        std::cerr << "Failed to open port \"" << name << "\"\n";
        return false;
    }

    return true;
}

void close_port()
{
    std::cerr << "UNIMPLEMENTED\n";
}
