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