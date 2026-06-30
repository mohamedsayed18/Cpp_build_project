add_library(plugin SHARED
    src/plugin.cpp
)

find_package(Boost REQUIRED COMPONENTS log log_setup system thread)

target_include_directories(plugin
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(plugin
    PRIVATE
        Boost::log
        Boost::log_setup
        Boost::system
        Boost::thread
)


target_compile_definitions(plugin
    PRIVATE PLUGIN_BUILDING
)

# Copy Conan runtime DLLs (e.g. Boost) next to plugin.dll on Windows so tests
# and the main executable can find them without extending PATH.
if(WIN32)
    add_custom_command(TARGET plugin POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:plugin>
            $<TARGET_FILE_DIR:plugin>
        COMMAND_EXPAND_LISTS
    )
endif()
