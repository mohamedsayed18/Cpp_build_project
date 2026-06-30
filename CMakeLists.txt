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
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Install Conan runtime shared libraries into the install tree.
# install(CODE) runs at install-time; it includes the Conan toolchain so that
# CONAN_RUNTIME_LIB_DIRS is available for globbing the actual package lib dirs.
# This avoids RUNTIME_DEPENDENCY_SET whose dependency scanner cannot locate
# libraries in the Conan package cache on CI runners.
install(CODE "
    cmake_policy(PUSH)
    include(\"${CMAKE_BINARY_DIR}/generators/conan_toolchain.cmake\" NO_POLICY_SCOPE)
    cmake_policy(POP)
    foreach(_lib_dir IN LISTS CONAN_RUNTIME_LIB_DIRS)
        if(WIN32)
            file(GLOB _runtime_libs \"\${_lib_dir}/*.dll\")
            if(_runtime_libs)
                file(INSTALL \${_runtime_libs}
                    DESTINATION \"\${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}\"
                )
            endif()
        else()
            file(GLOB _runtime_libs \"\${_lib_dir}/*.so*\")
            if(_runtime_libs)
                file(INSTALL \${_runtime_libs}
                    DESTINATION \"\${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}\"
                )
            endif()
        endif()
    endforeach()
")

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
