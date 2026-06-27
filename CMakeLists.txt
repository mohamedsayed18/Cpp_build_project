cmake_minimum_required(VERSION 3.21)

project(challenge_project LANGUAGES CXX)

include(CTest)
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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
