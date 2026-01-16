#include <klogger/logger.h>
#include <cassert>
#include <fstream>
#include <string>
#include <filesystem>
#include <iostream>

using namespace klogger;

static const char* LOG_FILE = "klogger_test.log";

void cleanup() {
    std::error_code ec;
    std::filesystem::remove(LOG_FILE, ec);
}

void test_init_and_basic_log() {
    cleanup();

    Logger::instance().init(LOG_FILE);

    LOG_DEBUG("debug test");    
    LOG_INFO("hello unit test");
    LOG_WARN("warn test");
    LOG_ERROR("error test");
    LOG_FATAL("fatal test");    

    std::ifstream file(LOG_FILE);
    assert(file.is_open());

    std::string line;
    bool found = false;

    while (std::getline(file, line)) {
        if (line.find("hello unit test") != std::string::npos) {
            found = true;
            break;
        }
    }

    assert(found && "Log message not written to file");
}

void test_exception_logging() {
    try {
        throw std::runtime_error("test exception");
    } catch (const std::exception& e) {
        LOG_EXCEPTION(e);
    }

    std::ifstream file(LOG_FILE);
    std::string line;
    bool found = false;

    while (std::getline(file, line)) {
        if (line.find("test exception") != std::string::npos) {
            found = true;
            break;
        }
    }

    assert(found && "Exception not logged");
}

void test_unknown_exception() {
    LOG_UNKNOWN_EXCEPTION();

    std::ifstream file(LOG_FILE);
    std::string line;
    bool found = false;

    while (std::getline(file, line)) {
        if (line.find("Unknown exception") != std::string::npos) {
            found = true;
            break;
        }
    }

    assert(found && "Unknown exception not logged");
}

void test_logfile_open_fail() {
    Logger::instance().init("/invalid/path/for/log.txt");

    LOG_INFO("should still write to stderr");
}

int main() {
    test_init_and_basic_log();
    test_exception_logging();
    test_unknown_exception();
    test_logfile_open_fail();

    Logger::instance().shutdown();

    std::cout << "All klogger unit tests passed ✅\n";
    return 0;
}
