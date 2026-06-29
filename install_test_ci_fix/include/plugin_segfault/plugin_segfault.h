#pragma once

#ifdef _WIN32
    #ifdef PLUGIN_SEGFAULT_BUILDING
        #define PLUGIN_SEGFAULT_API __declspec(dllexport)
    #else
        #define PLUGIN_SEGFAULT_API __declspec(dllimport)
    #endif
#else
    #define PLUGIN_SEGFAULT_API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

PLUGIN_SEGFAULT_API int plugin_segfault_init(const char* output_path);

PLUGIN_SEGFAULT_API const char* plugin_segfault_get_name(void);

PLUGIN_SEGFAULT_API void plugin_segfault_trigger_test_crash(void);

#ifdef __cplusplus
}
#endif