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
