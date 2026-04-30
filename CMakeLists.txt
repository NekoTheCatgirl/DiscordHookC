cmake_minimum_required(VERSION 4.2)
project(DiscordHookC
        VERSION 1.0.0.0
        DESCRIPTION "A C library for Discord Webhooks"
        HOMEPAGE_URL "https://github.com/NekoTheCatgirl/DiscordHookC"
        LANGUAGES C CXX
)

set(CMAKE_C_STANDARD 23)

find_package(CURL REQUIRED)
find_package(json-c REQUIRED)
find_package(Doxygen)

if (DOXYGEN_FOUND)
    set(DOXYGEN_IN "${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile")
    doxygen_add_docs(doc
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${CMAKE_CURRENT_SOURCE_DIR}/src
            ${CMAKE_CURRENT_SOURCE_DIR}/README.md
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc
            COMMENT "Generating API documentation with Doxygen"
    )
endif ()

add_library(DiscordHookC
        src/dishook.c
        src/color.c
)

target_include_directories(DiscordHookC
        PUBLIC include/
        PRIVATE src/
)

target_link_libraries(DiscordHookC
        PRIVATE CURL::libcurl
        PRIVATE json-c::json-c
)

install(TARGETS DiscordHookC
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
)

install(DIRECTORY include/
        DESTINATION include/dishook
)

option(DISHOOK_BUILD_EXAMPLES "Build examples" OFF)
option(DISHOOK_BUILD_TESTS "Build tests" OFF)

macro(load_dotenv)
    if (EXISTS "${CMAKE_SOURCE_DIR}/.env")
        file(STRINGS "${CMAKE_SOURCE_DIR}/.env" DOTENV_LINES)
        foreach(line ${DOTENV_LINES})
            if(line MATCHES "^([^=]+)=(.*)$")
                set(ENV_VAR_NAME ${CMAKE_MATCH_1})
                set(ENV_VAR_VALUE ${CMAKE_MATCH_2})
                # Remove quotes if present
                string(REGEX REPLACE "^\"(.*)\"$" "\\1" ENV_VAR_VALUE "${ENV_VAR_VALUE}")
                string(REGEX REPLACE "^'(.*)'$" "\\1" ENV_VAR_VALUE "${ENV_VAR_VALUE}")
                set(ENV{${ENV_VAR_NAME}} "${ENV_VAR_VALUE}")
                add_compile_definitions(${ENV_VAR_NAME}="${ENV_VAR_VALUE}")
            endif()
        endforeach()
    endif ()
endmacro()

if (DISHOOK_BUILD_EXAMPLES)
    load_dotenv()
    add_executable(simple_example
            example/simple_example.c
    )

    target_link_libraries(simple_example
            PRIVATE DiscordHookC
    )

    add_executable(embed_example
            example/embed_example.c
    )

    target_link_libraries(embed_example
            PRIVATE DiscordHookC
    )
endif ()

if (DISHOOK_BUILD_TESTS)
    enable_testing()
    load_dotenv()
    add_executable(test_basic
            tests/test_basic.c
    )
    target_link_libraries(test_basic
            PRIVATE DiscordHookC
    )
    add_test(NAME basic_test COMMAND test_basic)

    add_executable(test_env
            tests/test_env.c
    )
    target_link_libraries(test_env
            PRIVATE DiscordHookC
    )
    add_test(NAME env_test COMMAND test_env)
endif ()