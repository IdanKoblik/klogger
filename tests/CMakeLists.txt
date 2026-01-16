add_executable(klogger_tests
    test_logger.cpp
)

target_link_libraries(klogger_tests
    PRIVATE
        klogger::klogger
)

target_compile_features(klogger_tests PRIVATE cxx_std_17)

if (KLOGGER_ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(klogger_tests PRIVATE --coverage -O0 -g)
    target_link_options(klogger_tests PRIVATE --coverage)
endif()

add_test(
    NAME klogger_basic_tests
    COMMAND klogger_tests
)
