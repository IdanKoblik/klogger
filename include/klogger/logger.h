#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <exception>

namespace klogger {

enum class Level {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
};

class Logger {
public:
    static Logger& instance();

    void init(const std::string& logFile = "logs.txt");
    void shutdown();

    void log(Level level, const std::string& msg, const std::string& file = "", int line = 0);

    void logException(const std::exception& e, const std::string& file = "", int line = 0);

    void logUnknownException(const std::string& file = "", int line = 0);
private:
    Logger() = default;
    ~Logger() = default;

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::string levelToString(Level level);
    std::string getTimestamp();
    void writeLog(const std::string& logMessage);

    std::ofstream logFile;
    std::mutex mutex;
    bool initialized = false;
};

#define LOG_DEBUG(msg) klogger::Logger::instance().log(klogger::Level::DEBUG, msg, __FILE__, __LINE__)
#define LOG_INFO(msg)  klogger::Logger::instance().log(klogger::Level::INFO,  msg, __FILE__, __LINE__)
#define LOG_WARN(msg)  klogger::Logger::instance().log(klogger::Level::WARN,  msg, __FILE__, __LINE__)
#define LOG_ERROR(msg) klogger::Logger::instance().log(klogger::Level::ERROR, msg, __FILE__, __LINE__)
#define LOG_FATAL(msg) klogger::Logger::instance().log(klogger::Level::FATAL, msg, __FILE__, __LINE__)

#define LOG_EXCEPTION(e) klogger::Logger::instance().logException(e, __FILE__, __LINE__)
#define LOG_UNKNOWN_EXCEPTION() klogger::Logger::instance().logUnknownException(__FILE__, __LINE__)

}

#endif // LOGGER_H