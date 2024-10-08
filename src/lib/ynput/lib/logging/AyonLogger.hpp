#ifndef AYON_CPPTOOLS_BUILD_LOGGER
    #define AYONLOGGER_H
#endif   // DEBUG

#ifndef AYONLOGGER_H
    #define AYONLOGGER_H

    #include "spdlog/async.h"
    #include "spdlog/common.h"
    #include "spdlog/sinks/basic_file_sink.h"
    #include "spdlog/sinks/stdout_color_sinks.h"
    #include "spdlog/sinks/ostream_sink.h"
    #include <filesystem>
    #include <iostream>
    #include <memory>
    #include <set>
    #include <spdlog/spdlog.h>
    #include <string>

    #ifdef AYON_TEST_BUILD

std::ostringstream AyonTestOss;
    #endif   // AYON_TEST_BUILD

// TODO document Logger
/**
 * @class AyonLogger
 * @brief Simple Logger Class that wraps around spdlog in order to expose easy
 * logging functions \n AyonLogger::getInstance(log_File_path.json) init code \n
 *  automaticly logs to file and console
 */

class AyonLogger {
    public:
        static AyonLogger &
        getInstance(const std::string &filepath) {
            static AyonLogger AyonLoggerInstance(filepath);
            return AyonLoggerInstance;
        };

        std::set<std::string>::iterator
        key(const std::string &key) {
            return this->m_EnabledLoggingKeys.find(key);
        };

        bool
        regesterLoggingKey(const std::string &KeyName) {
            std::pair<std::set<std::string>::iterator, bool> insertion = this->m_EnabledLoggingKeys.insert(KeyName);
            if (insertion.second) {
                return true;
            }
            return false;
        };

        bool
        unregisterLoggingKey(const std::string &KeyName) {
            std::set<std::string>::iterator it = this->m_EnabledLoggingKeys.find(KeyName);
            if (it != this->m_EnabledLoggingKeys.end()) {
                this->m_EnabledLoggingKeys.erase(it);
                return true;
            }
            return false;
        };

        bool
        isKeyActive(const std::set<std::string>::iterator &logginIterator) {
            if (logginIterator != this->m_EnabledLoggingKeys.end()) {
                return true;
            }
            return false;
        };

        template<typename... Args>
        void
        error(const std::set<std::string>::iterator &logginIterator, const std::string &format, const Args &... args) {
            if (logginIterator != this->m_EnabledLoggingKeys.end()) {
                log("error", format, args...);
            }
        };

        template<typename... Args>
        void
        error(const std::string &format, const Args &... args) {
            log("error", format, args...);
        };

        template<typename... Args>
        void
        info(const std::set<std::string>::iterator &logginIterator, const std::string &format, const Args &... args) {
            if (logginIterator != this->m_EnabledLoggingKeys.end()) {
                log("info", format, args...);
            }
        };

        template<typename... Args>
        void
        info(const std::string &format, const Args &... args) {
            log("info", format, args...);
        };

        template<typename... Args>
        void
        warn(const std::set<std::string>::iterator &logginIterator, const std::string &format, const Args &... args) {
            if (logginIterator != this->m_EnabledLoggingKeys.end()) {
                log("warn", format, args...);
            }
        };

        template<typename... Args>
        void
        warn(const std::string &format, const Args &... args) {
            log("warn", format, args...);
        };

        template<typename... Args>
        void
        critical(const std::set<std::string>::iterator &logginIterator,
                 const std::string &format,
                 const Args &... args) {
            if (logginIterator != this->m_EnabledLoggingKeys.end()) {
                log("critical", format, args...);
            }
        };

        template<typename... Args>
        void
        critical(const std::string &format, const Args &... args) {
            log("critical", format, args...);
        };

        void
        LogLevlInfo(const bool &alsoSetFileLogger = false) {
            if (alsoSetFileLogger) {
                this->m_FileLogger->set_level(spdlog::level::info);
            }
            this->m_ConsoleLogger->set_level(spdlog::level::info);
        };

        void
        LogLevlError(const bool &alsoSetFileLogger = false) {
            if (alsoSetFileLogger) {
                this->m_FileLogger->set_level(spdlog::level::err);
            }
            this->m_ConsoleLogger->set_level(spdlog::level::err);
        };

        void
        LogLevlWarn(const bool &alsoSetFileLogger = false) {
            if (alsoSetFileLogger) {
                this->m_FileLogger->set_level(spdlog::level::warn);
            }
            this->m_ConsoleLogger->set_level(spdlog::level::warn);
        };

        void
        LogLevlCritical(const bool &alsoSetFileLogger = false) {
            if (alsoSetFileLogger) {
                this->m_FileLogger->set_level(spdlog::level::critical);
            }
            this->m_ConsoleLogger->set_level(spdlog::level::critical);
        };

        void
        LogLevlOff(const bool &alsoSetFileLogger = false) {
            if (alsoSetFileLogger) {
                this->m_FileLogger->set_level(spdlog::level::off);
            }
            this->m_ConsoleLogger->set_level(spdlog::level::off);
        };

    private:
        AyonLogger(const std::string &filepath) {
            this->m_ConsoleLogger = spdlog::stdout_color_mt("console");
            this->m_ConsoleLogger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");

    #ifdef AYON_TEST_BUILD
            auto ostream_sink = std::make_shared<spdlog::sinks::ostream_sink_st>(AyonTestOss);
            this->m_ConsoleLogger = std::make_shared<spdlog::logger>("root", ostream_sink);
    #endif   // AYON_TEST_BUILD

            if (!filepath.empty()) {
                this->m_EnableFileLogging = true;
                this->m_FileLogger = spdlog::basic_logger_mt<spdlog::async_factory>(
                    "fileLogger", std::filesystem::absolute(filepath.c_str()));

                this->m_FileLogger->set_pattern(
                    "{\"timestamp\":\"%Y-%m-%d %H:%M:%S.%e\",\"level\":\"%l\",\"Thread "
                    "Id\":\"%t\",\"Process Id\":\"%P\",\"message\":\"%v\"}");
            }
            else {
                this->m_EnableFileLogging = false;
            }
        };

        template<typename... Args>
        void
        log(const std::string &level, const std::string &massage, const Args &... args) {
            std::string formatted_message = fmt::vformat(massage, fmt::make_format_args(args...));

            if (this->m_EnableFileLogging) {
                this->m_FileLogger->log(spdlog::level::from_str(level), formatted_message);
            }
            this->m_ConsoleLogger->log(spdlog::level::from_str(level), formatted_message);
        };

        std::shared_ptr<spdlog::logger> m_ConsoleLogger;
        std::shared_ptr<spdlog::logger> m_FileLogger;
        // TODO use std::optional for this
        bool m_EnableFileLogging;
        std::string m_FileLoggerFilePath;

        std::set<std::string> m_EnabledLoggingKeys;
};
#endif
