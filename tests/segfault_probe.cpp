#include "plugin_segfault/plugin_segfault.h"

#include <filesystem>
#include <iostream>
#include <string>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

using plugin_init_fn = int (*)(const char*);
using plugin_crash_fn = void (*)();

class PluginLoader {
public:
    explicit PluginLoader(const std::string& path) {
#ifdef _WIN32
        handle_ = LoadLibraryA(path.c_str());
#else
        handle_ = dlopen(path.c_str(), RTLD_LAZY);
#endif
    }

    ~PluginLoader() {
        if (!handle_) return;
#ifdef _WIN32
        FreeLibrary(static_cast<HMODULE>(handle_));
#else
        dlclose(handle_);
#endif
    }

    template <typename T>
    T get_symbol(const char* name) const {
        if (!handle_) return nullptr;
#ifdef _WIN32
        return reinterpret_cast<T>(GetProcAddress(static_cast<HMODULE>(handle_), name));
#else
        return reinterpret_cast<T>(dlsym(handle_, name));
#endif
    }

    [[nodiscard]] bool is_loaded() const { return handle_ != nullptr; }

private:
    void* handle_ = nullptr;
};

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: segfault_probe <plugin_path> <stacktrace_path>" << std::endl;
        return 2;
    }

    const std::filesystem::path plugin_path = argv[1];
    const std::filesystem::path stacktrace_path = argv[2];

    PluginLoader loader(plugin_path.string());
    if (!loader.is_loaded()) {
        std::cerr << "Failed to load plugin_segfault" << std::endl;
        return 3;
    }

    auto init_fn = loader.get_symbol<plugin_init_fn>("plugin_segfault_init");
    auto crash_fn = loader.get_symbol<plugin_crash_fn>("plugin_segfault_trigger_test_crash");
    if (!init_fn || !crash_fn) {
        std::cerr << "Failed to resolve plugin_segfault symbols" << std::endl;
        return 4;
    }

    if (init_fn(stacktrace_path.string().c_str()) != 0) {
        std::cerr << "plugin_segfault_init failed" << std::endl;
        return 5;
    }

    crash_fn();
    std::cerr << "Expected crash did not happen" << std::endl;
    return 6;
}