add_library(plugin SHARED
    src/plugin.cpp
)

target_include_directories(plugin
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)


target_compile_definitions(plugin
    PRIVATE PLUGIN_BUILDING
)
