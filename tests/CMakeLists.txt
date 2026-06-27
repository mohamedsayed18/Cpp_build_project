find_package(GTest REQUIRED)

add_executable(plugin_tests
    test_plugin.cpp
)

target_include_directories(plugin_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/plugin/include
)

target_link_libraries(plugin_tests
    PRIVATE
        GTest::gtest_main
)

if(UNIX AND NOT APPLE)
    target_link_libraries(plugin_tests
        PRIVATE
            ${CMAKE_DL_LIBS}
    )
endif()

add_test(NAME plugin_tests COMMAND plugin_tests)
set_tests_properties(plugin_tests PROPERTIES
    ENVIRONMENT "PLUGIN_PATH=$<TARGET_FILE:plugin>"
)

add_executable(segfault_probe
    segfault_probe.cpp
)

target_include_directories(segfault_probe
    PRIVATE
        ${PROJECT_SOURCE_DIR}/plugin_segfault/include
)

if(UNIX AND NOT APPLE)
    target_link_libraries(segfault_probe
        PRIVATE
            ${CMAKE_DL_LIBS}
    )
endif()

add_executable(plugin_segfault_tests
    test_plugin_segfault.cpp
)

target_include_directories(plugin_segfault_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/plugin_segfault/include
)

target_link_libraries(plugin_segfault_tests
    PRIVATE
        GTest::gtest_main
)

if(UNIX AND NOT APPLE)
    target_link_libraries(plugin_segfault_tests
        PRIVATE
            ${CMAKE_DL_LIBS}
    )
endif()

add_test(NAME plugin_segfault_tests COMMAND plugin_segfault_tests)
set_tests_properties(plugin_segfault_tests PROPERTIES
    ENVIRONMENT
        "SEGFAULT_PROBE_PATH=$<TARGET_FILE:segfault_probe>;SEGFAULT_PLUGIN_PATH=$<TARGET_FILE:plugin_segfault>"
)