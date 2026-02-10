//
// Created by lv on 2/3/26.
//

#ifndef LAB3_1_SIMPLE_LOGGER_H
#define LAB3_1_SIMPLE_LOGGER_H

// #pragma once
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <mutex>

using namespace std;

namespace logging {

    enum class Level { TRACE, DEBUG, INFO, WARN, ERROR, FATAL };

    inline void init();
    inline void log(Level level, const string& message);


    inline void trace(const string& message);
    inline void debug(const string& message);
    inline void info(const string& message);
    inline void warn(const string& message);
    inline void error(const string& message, const string& file, int line);
    inline void fatal(const string& message);

    inline void setMinLevel(Level level);
    inline Level getMinLevel();


    #define LOG_TRACE(msg) ::logging::trace(msg)
    #define LOG_DEBUG(msg) ::logging::debug(msg)
    #define LOG_INFO(msg)  ::logging::info(msg)
    #define LOG_WARN(msg)  ::logging::warn(msg)
    #define LOG_ERROR(msg) ::logging::error(msg, __FILE__, __LINE__)
    #define LOG_FATAL(msg) ::logging::fatal(msg)


    namespace detail {

        struct LoggerState {
            Level minLevel = Level::TRACE;  // logs can get resource heavy
            bool showTimestamp = true;
            bool coloredOutput = true;
        };

        inline LoggerState& getState() {
            static LoggerState state;
            return state;
        }

        inline string levelToString(Level level) {
            switch (level) {
                case Level::TRACE: return "TRACE";
                case Level::DEBUG: return "DEBUG";
                case Level::INFO:  return "INFO";
                case Level::WARN:  return "WARN";
                case Level::ERROR: return "ERROR";
                case Level::FATAL: return "FATAL";
                default: return "UNKNOWN";
            }
        }

        inline string getCurrentTime() {
            auto now = chrono::system_clock::now();
            auto time = chrono::system_clock::to_time_t(now);
            auto ms = chrono::duration_cast<chrono::milliseconds>(
                now.time_since_epoch()) % 1000;

            stringstream ss;
            ss << put_time(localtime(&time), "%H:%M:%S")
               << '.' << setfill('0') << setw(3) << ms.count();
            return ss.str();
        }

        inline bool shouldLog(Level level) {
            auto& state = getState();
            return static_cast<int>(level) >= static_cast<int>(state.minLevel);
        }

        //colors
        inline string getColorCode(Level level) {
            auto& state = getState();


            if (!state.coloredOutput) return "";


            switch (level) {
                case Level::TRACE: return "\033[90m";  // серый
                case Level::DEBUG: return "\033[36m";  // голубой
                case Level::INFO:  return "\033[32m";  // зеленый
                case Level::WARN:  return "\033[33m";  // желтый
                case Level::ERROR: return "\033[31m";  // красный
                case Level::FATAL: return "\033[35m";  // фиолетовый
                default: return "\033[0m";            // сброс
                    /**
                     * The sequence \033[0m is the standard ANSI escape code to reset
                     * all text formatting and colors back to the terminal's default
                     * settings. **/
            }
        }

        inline string getResetColor() {
            auto& state = getState();
            return state.coloredOutput ? "\033[0m" : "";
        }

        inline mutex& getConsoleMutex() {
            static mutex consoleMutex;
            return consoleMutex;
        }

    } // namespace detail

    // ========== РЕАЛИЗАЦИИ ПУБЛИЧНЫХ ФУНКЦИЙ ==========

    inline void log(Level level, const string& message) {
        auto& state = detail::getState();

        if (!detail::shouldLog(level)) {
            return;
        }

        lock_guard<mutex> lock(detail::getConsoleMutex());

        // formatting of the log stringStream -> [11:59:56.816] [INFO] the logger app has initialized
        stringstream logEntry;

        if (state.showTimestamp) {
            logEntry << "[" << detail::getCurrentTime() << "] ";
        }

        logEntry << "[" << detail::levelToString(level) << "] "
                 << message;

        // Выбираем поток вывода
        ostream& outputStream = (level == Level::ERROR || level == Level::FATAL)
                               ? cerr : cout;

        if (state.coloredOutput) {
            outputStream << detail::getColorCode(level)
                        << logEntry.str()
                        << detail::getResetColor()
                        << endl;
        } else {
            outputStream << logEntry.str() << endl;
        }
    }


    inline void init() {
        log(Level::INFO, "the logger app has initialized");
    }

    inline void trace(const string& message) {
        log(Level::TRACE, message);
    }

    inline void debug(const string& message) {
        log(Level::DEBUG, message);
    }

    inline void info(const string& message) {
        log(Level::INFO, message);
    }

    inline void warn(const string& message) {
        log(Level::WARN, message);
    }


    inline void error(const string& message, const string& file, int line) {
        stringstream ss;

        string fileName = file; //formating the path
        size_t pos = fileName.find_last_of("/\\");
        if (pos != string::npos) {
            fileName = fileName.substr(pos + 1);
        }

        ss << message << " [at " << fileName << ":" << line << "]";
        log(Level::ERROR, ss.str());
    }

    inline void fatal(const string& message) {
        log(Level::FATAL, message);
    }

    inline void setMinLevel(Level level) {
        auto& state = detail::getState();
        state.minLevel = level;
    }

    inline Level getMinLevel() {
        auto& state = detail::getState();
        return state.minLevel;
    }

    // Дополнительные настройки
    inline void enableTimestamps(bool enable) {
        auto& state = detail::getState();
        state.showTimestamp = enable;
    }

    inline void enableColors(bool enable) {
        auto& state = detail::getState();
        state.coloredOutput = enable;
    }

} // namespace logging

#endif //LAB3_1_SIMPLE_LOGGER_H