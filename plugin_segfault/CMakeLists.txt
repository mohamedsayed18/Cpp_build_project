add_library(plugin_segfault SHARED
    src/plugin_segfault.cpp
)

target_include_directories(plugin_segfault
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(plugin_segfault
    PRIVATE PLUGIN_SEGFAULT_BUILDING
)

if(NOT MSVC)
    target_compile_options(plugin_segfault PRIVATE
        -fvisibility=hidden
        -fvisibility-inlines-hidden
    )
endif()