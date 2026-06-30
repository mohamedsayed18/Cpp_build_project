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
if(WIN32)
    # Copy GTest (and any other) runtime DLLs next to the test exe so Windows
    # finds them without needing an extended PATH.
    add_custom_command(TARGET plugin_tests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:plugin_tests>
            $<TARGET_FILE_DIR:plugin_tests>
        COMMAND_EXPAND_LISTS
    )
    set_tests_properties(plugin_tests PROPERTIES
        ENVIRONMENT
            "PLUGIN_PATH=$<TARGET_FILE:plugin>"
    )
else()
    set(_plugin_test_ld_path "$<TARGET_FILE_DIR:plugin>")
    if(CONAN_RUNTIME_LIB_DIRS)
        string(REPLACE ";" ":" _conan_runtime_ld_path "${CONAN_RUNTIME_LIB_DIRS}")
        set(_plugin_test_ld_path "${_plugin_test_ld_path}:${_conan_runtime_ld_path}")
    endif()

    set_tests_properties(plugin_tests PROPERTIES
        ENVIRONMENT
            "PLUGIN_PATH=$<TARGET_FILE:plugin>;LD_LIBRARY_PATH=${_plugin_test_ld_path}:$ENV{LD_LIBRARY_PATH}"
    )
endif()

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