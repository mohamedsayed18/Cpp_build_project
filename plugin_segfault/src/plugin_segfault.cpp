#include "plugin_segfault/plugin_segfault.h"

#include <array>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#else
#include <csignal>
#include <execinfo.h>
#include <unistd.h>
#endif

namespace {

    std::string g_stacktrace_path = "segfault_stacktrace.txt";

#ifdef _WIN32
    LONG WINAPI segfault_handler(EXCEPTION_POINTERS*)
    {
        constexpr USHORT max_frames = 64;
        std::array<void*, max_frames> frames{};
        const USHORT count = CaptureStackBackTrace(0, max_frames, frames.data(), nullptr);

        std::ofstream out(g_stacktrace_path, std::ios::out | std::ios::trunc);
        if (out) {
            out << "signal=access_violation\n";
            out << "frames=" << count << "\n";
            for (USHORT i = 0; i < count; ++i) {
                out << i << ":" << frames[i] << "\n";
            }
            out.flush();
            out.close();
        }

        return EXCEPTION_EXECUTE_HANDLER;
    }
#else
    void segfault_handler(int signum)
    {
        constexpr int max_frames = 64;
        std::array<void*, max_frames> frames{};
        const int count = backtrace(frames.data(), max_frames);
        char** symbols = backtrace_symbols(frames.data(), count);

        std::ofstream out(g_stacktrace_path, std::ios::out | std::ios::trunc);
        if (out) {
            out << "signal=" << signum << "\n";
            out << "frames=" << count << "\n";
            for (int i = 0; i < count; ++i) {
                if (symbols != nullptr) {
                    out << i << ":" << symbols[i] << "\n";
                }
                else {
                    out << i << ":<unknown>\n";
                }
            }
            out.flush();
            out.close();
        }

        std::free(symbols);
        _Exit(128 + signum);
    }
#endif

} // namespace

extern "C" {

PLUGIN_SEGFAULT_API int plugin_segfault_init(const char* output_path)
{
    if (output_path != nullptr && output_path[0] != '\0') {
        g_stacktrace_path = output_path;
    }

#ifdef _WIN32
    SetUnhandledExceptionFilter(segfault_handler);
#else
    std::signal(SIGSEGV, segfault_handler);
#endif

    return 0;
}

PLUGIN_SEGFAULT_API const char* plugin_segfault_get_name(void)
{
    return "plugin_segfault";
}

PLUGIN_SEGFAULT_API void plugin_segfault_trigger_test_crash(void)
{
    volatile int* ptr = nullptr;
    *ptr = 1;
}
}