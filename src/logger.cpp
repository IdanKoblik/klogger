#include <klogger/logger.h>
#include <iostream>
#include <ctime>

namespace klogger {

Logger& Logger::instance() {
    static Logger instance;
    return instance;
}

void Logger::init(const std::string& path) {
    std::lock_guard<std::mutex> lock(mutex);

    if (initialized)
        return;

    logFile.open(path, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "klogger: failed to open log file: " << path << '\n';
        return;
    }

    initialized = true;
}

void Logger::shutdown() {
    std::lock_guard<std::mutex> lock(mutex);

    if (logFile.is_open())
        logFile.close();

    initialized = false;
}

void Logger::log(Level level, const std::string& msg, const std::string& file, int line) {
    std::lock_guard<std::mutex> lock(mutex);

    std::ostringstream oss;
    oss << getTimestamp()
        << " [" << levelToString(level) << "]";

    if (!file.empty() && line > 0) {
        auto pos = file.find_last_of("/\\");
        oss << " [" << file.substr(pos + 1) << ":" << line << "]";
    }

    oss << " " << msg;

    writeLog(oss.str());
}

void Logger::logException(const std::exception& e, const std::string& file, int line) {
    log(Level::ERROR, std::string("Exception: ") + e.what(), file, line);
}

void Logger::logUnknownException(const std::string& file, int line) {
    log(Level::ERROR, "Unknown exception caught", file, line);
}

std::string Logger::levelToString(Level level) {
    switch (level) {
        case Level::DEBUG: return "DEBUG";
        case Level::INFO:  return "INFO";
        case Level::WARN:  return "WARN";
        case Level::ERROR: return "ERROR";
        case Level::FATAL: return "FATAL";
        default:           return "UNKNOWN";
    }
}

std::string Logger::getTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    std::tm tm{};
    localtime_r(&time, &tm);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S")
        << "." << std::setw(3) << std::setfill('0') << ms.count();

    return oss.str();
}

void Logger::writeLog(const std::string& message) {
    std::cerr << message << '\n';

    if (initialized && logFile.is_open()) {
        logFile << message << '\n';
        logFile.flush();
    }
}


}