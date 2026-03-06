//
// Created by Orik on 20/09/2025.
//

#ifndef MOTOR_LOGGER_H
#define MOTOR_LOGGER_H

#include <string>
#include <mutex>

#define LOG_INFO(msg) Motor::Logger::log(Motor::Logger::LogLevel::Info, __FILE__, __func__, msg);
#define LOG_WARN(msg) Motor::Logger::log(Motor::Logger::LogLevel::Warning, __FILE__, __func__, msg);
#define LOG_ERROR(msg) Motor::Logger::log(Motor::Logger::LogLevel::Error, __FILE__, __func__, msg);
#define LOG_FATAL(msg) Motor::Logger::log(Motor::Logger::LogLevel::Fatal, __FILE__, __func__, msg);

namespace Motor {
    class Logger {
    public:
        enum LogLevel {
            Info = 1 << 0,
            Warning = 1 << 1,
            Error = 1 << 2,
            Fatal = 1 << 3,
        };

        static void log(LogLevel level, const char* file, const char* function, const std::string& message);
        static void activateLevel(LogLevel level);
        static void deactivateLevel(LogLevel level);

    private:
        static std::mutex mutex;
        static int activeLevels;

        static std::string getLevelName(LogLevel level);
    };
}

#endif //MOTOR_LOGGER_H