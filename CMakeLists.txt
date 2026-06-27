cmake_minimum_required(VERSION 3.21)

project(challenge_project LANGUAGES CXX)

include(CTest)
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate compile_commands.json for clang-tidy
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})

add_subdirectory(plugin)
add_subdirectory(plugin_segfault)
add_subdirectory(app)

if(BUILD_TESTING)
	add_subdirectory(tests)
endif()

# Install rules for Task 4
# Enable RPATH on install so challenge finds plugins in ../lib
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

install(TARGETS challenge
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(TARGETS plugin plugin_segfault
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Collect and install Conan runtime dependencies to lib folder
if(DEFINED CONAN_RUNTIME_LIB_DIRS)
    install(RUNTIME_DEPENDENCY_SET conan_deps
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        PRE_EXCLUDE_REGEXES [[^/lib64/ld-linux-x86-64]] [[api-ms-win-.*]] [[ext-ms-win-.*]] [[kernel32\.dll]] [[libc\.so\..*]] [[libgcc_s\.so\..*]] [[libm\.so\..*]] [[libstdc\+\+\.so\.].*]
        POST_EXCLUDE_REGEXES [[.*/system32/.*\.dll]]
        DIRECTORIES ${CONAN_RUNTIME_LIB_DIRS}
    )
endif()

# Also install headers from plugins for completeness
install(DIRECTORY plugin/include/plugin
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY plugin_segfault/include/plugin_segfault
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Linter targets for Task 5
find_program(CLANG_FORMAT_EXECUTABLE clang-format)
find_program(CLANG_TIDY_EXECUTABLE clang-tidy)

if(CLANG_FORMAT_EXECUTABLE AND CLANG_TIDY_EXECUTABLE)
    add_custom_target(format
        COMMAND ${PROJECT_SOURCE_DIR}/scripts/format.sh
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Running clang-format to format all C++ files"
    )

    add_custom_target(lint
        COMMAND ${PROJECT_SOURCE_DIR}/scripts/lint.sh
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Running linters (clang-format) on all C++ files"
    )

    add_custom_target(clang-tidy
        COMMAND ${PROJECT_SOURCE_DIR}/scripts/clang-tidy.sh
        DEPENDS challenge plugin plugin_segfault plugin_tests segfault_probe plugin_segfault_tests
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Running clang-tidy on all C++ source files"
    )
else()
    message(STATUS "clang-format and/or clang-tidy not found: lint targets are disabled")
endif()
