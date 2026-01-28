#pragma once

#define SPDLOG_NO_ASYNC_LOGGER
#define SPDLOG_DISABLE_DEFAULT_LOGGER
#define SPDLOG_NO_ATOMIC_LEVELS

#include "spdlog/common.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <filesystem>
#include <memory>
#include <set>
#include <spdlog/spdlog.h>
#include <string>
#include <iostream> // Added for std::cout/cerr

class AyonLogger {
public:
    // Singleton Accessor (Parameterless to avoid confusion about re-init)
    static AyonLogger &getInstance() {
        static AyonLogger AyonLoggerInstance;
        return AyonLoggerInstance;
    };

    // Explicit Initialization for File Logging
    void initFileLogger(const std::string &filepath) {
        if (m_EnableFileLogging) {
            std::cout << "[AyonLogger] File logger already enabled. Ignoring new path: " << filepath << std::endl;
            return;
        }

        if (filepath.empty()) return;

        std::cout << "[AyonLogger] Initializing file logger at: " << filepath << std::endl;
        try {
            auto abs_path = std::filesystem::absolute(filepath).string();
            
            // Create sink
            auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(abs_path, true);
            file_sink->set_pattern("{\"timestamp\":\"%Y-%m-%d %H:%M:%S.%e\",\"level\":\"%l\",\"Thread Id\":\"%t\",\"Process Id\":\"%P\",\"message\":\"%v\"}");

            // Create Logger
            m_FileLogger = std::make_shared<spdlog::logger>("file_logger", file_sink);
            m_FileLogger->set_level(spdlog::level::info);
            
            // CRITICAL FIX: Flush immediately on info to capture logs before crash
            m_FileLogger->flush_on(spdlog::level::info);

            m_EnableFileLogging = true;
            std::cout << "[AyonLogger] File logger initialized successfully." << std::endl;
        }
        catch (const std::exception &e) {
            std::cerr << "[AyonLogger] Failed to init file logger: " << e.what() << std::endl;
        }
    }

    // --- Singleton Safety ---
    AyonLogger(const AyonLogger&) = delete;            // Prevent Copying
    AyonLogger& operator=(const AyonLogger&) = delete; // Prevent Assignment

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
    // Private Constructor (Runs once)
    AyonLogger() {
        std::cout << "[AyonLogger] Singleton Constructor Started." << std::endl;
        
        // Console logger (always active)
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
        m_ConsoleLogger = std::make_shared<spdlog::logger>("console", console_sink);
        m_ConsoleLogger->set_level(spdlog::level::info);
        
        std::cout << "[AyonLogger] Singleton Constructor Finished." << std::endl;
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

    std::shared_ptr<spdlog::logger> m_ConsoleLogger;
    std::shared_ptr<spdlog::logger> m_FileLogger;

    bool m_EnableFileLogging{false};
    std::set<std::string> m_EnabledLoggingKeys;
};
