#pragma once

// #include "spdlog/async.h"
#include "spdlog/common.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <filesystem>
#include <memory>
#include <set>
#include <spdlog/spdlog.h>
#include <string>

class AyonLogger {
public:
    static AyonLogger &getInstance(const std::string &filepath) {
        static AyonLogger AyonLoggerInstance(filepath);
        return AyonLoggerInstance;
    };

    std::set<std::string>::iterator key(const std::string &key) {
        return m_EnabledLoggingKeys.find(key);
    };

    bool registerLoggingKey(const std::string &keyName) {
        auto insertion = m_EnabledLoggingKeys.insert(keyName);
        return insertion.second;
    };

    bool unregisterLoggingKey(const std::string &keyName) {
        auto it = m_EnabledLoggingKeys.find(keyName);
        if (it != m_EnabledLoggingKeys.end()) {
            m_EnabledLoggingKeys.erase(it);
            return true;
        }
        return false;
    };

    bool isKeyActive(const std::set<std::string>::iterator &loggingIterator) {
        return loggingIterator != m_EnabledLoggingKeys.end();
    };

    template<typename... Args>
    void error(fmt::format_string<Args...> fmt, Args&&... args) {
        log(spdlog::level::err, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void error(const std::set<std::string>::iterator &it,
               fmt::format_string<Args...> fmt, Args&&... args) {
        if (isKeyActive(it))
            log(spdlog::level::err, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void info(fmt::format_string<Args...> fmt, Args&&... args) {
        log(spdlog::level::info, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void info(const std::set<std::string>::iterator &it,
              fmt::format_string<Args...> fmt, Args&&... args) {
        if (isKeyActive(it))
            log(spdlog::level::info, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void warn(fmt::format_string<Args...> fmt, Args&&... args) {
        log(spdlog::level::warn, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void warn(const std::set<std::string>::iterator &it,
              fmt::format_string<Args...> fmt, Args&&... args) {
        if (isKeyActive(it))
            log(spdlog::level::warn, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void critical(fmt::format_string<Args...> fmt, Args&&... args) {
        log(spdlog::level::critical, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void critical(const std::set<std::string>::iterator &it,
                  fmt::format_string<Args...> fmt, Args&&... args) {
        if (isKeyActive(it))
            log(spdlog::level::critical, fmt, std::forward<Args>(args)...);
    }

    void LogLevelInfo(bool applyToFile = false) {
        setLevel(spdlog::level::info, applyToFile);
    }
    void LogLevelError(bool applyToFile = false) {
        setLevel(spdlog::level::err, applyToFile);
    }
    void LogLevelWarn(bool applyToFile = false) {
        setLevel(spdlog::level::warn, applyToFile);
    }
    void LogLevelCritical(bool applyToFile = false) {
        setLevel(spdlog::level::critical, applyToFile);
    }
    void LogLevelOff(bool applyToFile = false) {
        setLevel(spdlog::level::off, applyToFile);
    }

private:
    AyonLogger(const std::string &filepath) {
        m_ConsoleLogger = spdlog::stdout_color_mt(filepath + "_console");
        m_ConsoleLogger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");

        if (!filepath.empty()) {
            m_EnableFileLogging = true;
            try {
                m_FileLogger = spdlog::basic_logger_mt(
                    filepath + "_file",
                    std::filesystem::absolute(filepath).string());

                m_FileLogger->set_pattern(
                    "{\"timestamp\":\"%Y-%m-%d %H:%M:%S.%e\","
                    "\"level\":\"%l\",\"thread_id\":\"%t\","
                    "\"process_id\":\"%P\",\"message\":\"%v\"}");
            }
            catch (const std::exception &e) {
                m_EnableFileLogging = false;
                m_ConsoleLogger->warn("Failed to init file logger '{}': {}", filepath, e.what());
            }
        }
    }

    template<typename... Args>
    void log(spdlog::level::level_enum lvl,
             fmt::format_string<Args...> fmt,
             Args&&... args)
    {
        if (m_ConsoleLogger)
            m_ConsoleLogger->log(lvl, fmt, std::forward<Args>(args)...);

        if (m_EnableFileLogging && m_FileLogger)
            m_FileLogger->log(lvl, fmt, std::forward<Args>(args)...);
    }

    void setLevel(spdlog::level::level_enum lvl, bool applyToFile) {
        if (m_ConsoleLogger)
            m_ConsoleLogger->set_level(lvl);
        if (applyToFile && m_FileLogger)
            m_FileLogger->set_level(lvl);
    }

private:
    std::shared_ptr<spdlog::logger> m_ConsoleLogger;
    std::shared_ptr<spdlog::logger> m_FileLogger;

    bool m_EnableFileLogging{false};
    std::set<std::string> m_EnabledLoggingKeys;
};

