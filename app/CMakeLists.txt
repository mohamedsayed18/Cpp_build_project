add_executable(challenge
    src/main.cpp
)

target_include_directories(challenge
    PRIVATE
        ${PROJECT_SOURCE_DIR}/plugin/include
)

if(UNIX AND NOT APPLE)
    target_link_libraries(challenge
        PRIVATE
            ${CMAKE_DL_LIBS}
    )
endif()