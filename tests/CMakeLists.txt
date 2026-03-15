cmake_minimum_required(VERSION 3.16)

project(markdown_parser_tests LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

file(GLOB_RECURSE MARKDOWN_PARSER_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/../module/c_src/*.c"
)

add_library(markdown_parser ${MARKDOWN_PARSER_SOURCES})

set_target_properties(markdown_parser PROPERTIES
    OUTPUT_NAME markdown_parser
)

target_include_directories(markdown_parser
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../module/c_src/lib
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../module/c_src
        ${CMAKE_CURRENT_SOURCE_DIR}/../module/c_src/m_str
        ${CMAKE_CURRENT_SOURCE_DIR}/../module/c_src/m_html
        ${CMAKE_CURRENT_SOURCE_DIR}/../module/c_src/m_html/core
        ${CMAKE_CURRENT_SOURCE_DIR}/../module/c_src/m_html/escape
        ${CMAKE_CURRENT_SOURCE_DIR}/../module/c_src/m_html/attrs
        ${CMAKE_CURRENT_SOURCE_DIR}/../module/c_src/m_html/tags
        ${CMAKE_CURRENT_SOURCE_DIR}/../module/c_src/m_html/blocks
        ${CMAKE_CURRENT_SOURCE_DIR}/../module/c_src/m_html/inline
        ${CMAKE_CURRENT_SOURCE_DIR}/../module/c_src/m_html/elements
        ${CMAKE_CURRENT_SOURCE_DIR}/../module/c_src/m_md
        ${CMAKE_CURRENT_SOURCE_DIR}/../module/c_src/m_md/shared
        ${CMAKE_CURRENT_SOURCE_DIR}/../module/c_src/m_md/blocks
        ${CMAKE_CURRENT_SOURCE_DIR}/../module/c_src/m_md/document
        ${CMAKE_CURRENT_SOURCE_DIR}/../module/c_src/m_md/inline
)

add_executable(markdown_parser_test
    ${CMAKE_CURRENT_SOURCE_DIR}/C/test_main.c
)

set_target_properties(markdown_parser_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/C
)

target_include_directories(markdown_parser_test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../module/c_src
        ${CMAKE_CURRENT_SOURCE_DIR}/../module/c_src/lib
)

target_link_libraries(markdown_parser_test
    PRIVATE
        markdown_parser
)

if(MSVC)
    target_compile_options(markdown_parser PRIVATE /W4)
    target_compile_options(markdown_parser_test PRIVATE /W4)
else()
    target_compile_options(markdown_parser PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(markdown_parser_test PRIVATE -Wall -Wextra -Wpedantic)
endif()