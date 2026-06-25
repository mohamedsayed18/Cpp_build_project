find_package(PkgConfig REQUIRED)
pkg_check_modules(GTEST REQUIRED gtest gtest_main)

add_executable(plugin_tests
    test_plugin.cpp
)

target_include_directories(plugin_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/plugin/include
        ${GTEST_INCLUDE_DIRS}
)

target_compile_options(plugin_tests
    PRIVATE
        ${GTEST_CFLAGS_OTHER}
)

target_link_directories(plugin_tests
    PRIVATE
        ${GTEST_LIBRARY_DIRS}
)

target_link_libraries(plugin_tests
    PRIVATE
        ${GTEST_LIBRARIES}
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