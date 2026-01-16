cmake_minimum_required(VERSION 3.20)
project(klogger VERSION 1 LANGUAGES CXX)

option(KLOGGER_ENABLE_COVERAGE "Enable code coverage" OFF)

add_library(klogger STATIC
    src/logger.cpp
)

add_library(klogger::klogger ALIAS klogger)

target_include_directories(klogger
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(klogger PUBLIC cxx_std_17)
target_compile_options(klogger PRIVATE -Wall -Wextra -Wpedantic)

if (KLOGGER_ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    message(STATUS "Code coverage enabled")
    target_compile_options(klogger PRIVATE --coverage -O0 -g)
    target_link_options(klogger PRIVATE --coverage)
endif()

include(CTest)
enable_testing()
add_subdirectory(tests)

if (KLOGGER_ENABLE_COVERAGE)
    find_program(LCOV lcov)
    find_program(GENHTML genhtml)

    if (LCOV AND GENHTML)
        add_custom_target(coverage
            COMMAND ${LCOV} --directory . --capture --output-file coverage.info
            COMMAND ${LCOV} --remove coverage.info
                '/usr/*'
                '${CMAKE_BINARY_DIR}/*'
                '*/tests/*'
                --ignore-errors unused
                --output-file coverage.info
            COMMAND ${GENHTML} coverage.info --output-directory coverage-report
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Generating code coverage report"
        )
    else()
        message(WARNING "lcov/genhtml not found, coverage target disabled")
    endif()
endif()
