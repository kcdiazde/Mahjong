#pragma once

#include <cstdarg>
#include <cstdio>
#include <iostream>
#include <mutex>
#include <string>

enum class LogLevel {
    kQuiet,
    kInfo,
    kDebug,
    kVerbose,
};

class Logger {
  public:
    static Logger &Instance() {
        static Logger instance;
        return instance;
    }

    void SetLevel(LogLevel level) { level_ = level; }

    void Info(const char *format, ...) {
        if (level_ >= LogLevel::kInfo) {
            va_list args;
            va_start(args, format);
            PrintWithLevel("INFO", format, args);
            va_end(args);
        }
    }

    void Debug(const char *format, ...) {
        if (level_ >= LogLevel::kDebug) {
            va_list args;
            va_start(args, format);
            PrintWithLevel("DEBUG", format, args);
            va_end(args);
        }
    }

    void Verbose(const char *format, ...) {
        if (level_ >= LogLevel::kVerbose) {
            va_list args;
            va_start(args, format);
            PrintWithLevel("VERBOSE", format, args);
            va_end(args);
        }
    }

  private:
    LogLevel level_ = LogLevel::kQuiet;
    std::mutex mutex_;

    Logger() = default;
    ~Logger() = default;
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    void PrintWithLevel(const char *level_name, const char *format,
                        va_list args) {
        std::lock_guard<std::mutex> lock(mutex_); // thread-safe
        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), format, args);
        std::cout << "[" << level_name << "] " << buffer << std::endl;
    }
};
