
#pragma once

#include <iostream>
#include <string>
#include <cstdarg>
#include <cstdio>
#include <mutex>

enum class LogLevel {
    QUIET,
    INFO,
    DEBUG,
    VERBOSE 
};

class Logger {
public:
    static Logger& instance() {
        static Logger _instance;
        return _instance;
    }

    void set_level(LogLevel level) {
        _level = level;
    }

    void info(const char* format, ...) {
        if (_level >= LogLevel::INFO) {
            va_list args;
            va_start(args, format);
            print_with_level("INFO", format, args);
            va_end(args);
        }
    }

    void debug(const char* format, ...) {
        if (_level >= LogLevel::DEBUG) {
            va_list args;
            va_start(args, format);
            print_with_level("DEBUG", format, args);
            va_end(args);
        }
    }

    void verbose(const char* format, ...) {
        if (_level >= LogLevel::VERBOSE) {
            va_list args;
            va_start(args, format);
            print_with_level("VERBOSE", format, args);
            va_end(args);
        }
    }

private:
    LogLevel _level = LogLevel::QUIET;
    std::mutex _mutex;

    Logger() = default;
    ~Logger() = default;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void print_with_level(const char* level_name, const char* format, va_list args) {
        std::lock_guard<std::mutex> lock(_mutex); // thread-safe
        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), format, args);
        std::cout << "[" << level_name << "] " << buffer << std::endl;
    }
};
