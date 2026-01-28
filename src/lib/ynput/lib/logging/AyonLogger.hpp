#pragma once

#include "spdlog/async.h"
#include "spdlog/common.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <filesystem>
#include <memory>
#include <set>
#include <spdlog/spdlog.h>
#include <string>
#include <iostream>

/**
 * @class AyonLogger
 * @brief Simple Logger Class that wraps around spdlog in order to expose easy
 * logging functions. Uses async logging for better performance.
 * 
 * Usage:
 *   auto& log = AyonLogger::getInstance();
 *   log.initFileLogger("/tmp/app.log");
 *   log.registerLoggingKey("MyModule");
 *   log.info(log.key("MyModule"), "Started {}", version);
 */
class AyonLogger {
public:
    // Singleton Accessor (Parameterless to avoid confusion about re-init)
    static AyonLogger& getInstance() {
        static AyonLogger instance;
        return instance;
    }

    // Explicit Initialization for File Logging
    void initFileLogger(const std::string& filepath) {
        if (m_enableFileLogging) {
            std::cout << "[AyonLogger] File logger already enabled. Ignoring new path: " 
                      << filepath << std::endl;
            return;
        }

        if (filepath.empty()) return;

        std::cout << "[AyonLogger] Initializing async file logger at: " << filepath << std::endl;
        try {
            // Initialize async thread pool if not already done
            static bool pool_initialized = false;
            if (!pool_initialized) {
                spdlog::init_thread_pool(8192, 1);
                pool_initialized = true;
            }

            auto abs_path = std::filesystem::absolute(filepath).string();
            
            // Create async file logger
            m_fileLogger = spdlog::basic_logger_mt<spdlog::async_factory>(
                "file_logger", abs_path);
            
            m_fileLogger->set_pattern(
                "{\"timestamp\":\"%Y-%m-%d %H:%M:%S.%e\",\"level\":\"%l\","
                "\"thread_id\":\"%t\",\"process_id\":\"%P\",\"message\":\"%v\"}");
            m_fileLogger->set_level(spdlog::level::info);
            
            // Flush immediately on info level to capture logs before crash
            m_fileLogger->flush_on(spdlog::level::info);

            m_enableFileLogging = true;
            std::cout << "[AyonLogger] Async file logger initialized successfully." << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "[AyonLogger] Failed to init async file logger: " << e.what() << std::endl;
            m_enableFileLogging = false;
        }
    }

    // --- Singleton Safety ---
    AyonLogger(const AyonLogger&) = delete;
    AyonLogger& operator=(const AyonLogger&) = delete;

    std::set<std::string>::iterator key(const std::string& key) {
        return m_enabledLoggingKeys.find(key);
    }

    bool registerLoggingKey(const std::string& keyName) {
        auto insertion = m_enabledLoggingKeys.insert(keyName);
        return insertion.second;
    }

    bool unregisterLoggingKey(const std::string& keyName) {
        auto it = m_enabledLoggingKeys.find(keyName);
        if (it != m_enabledLoggingKeys.end()) {
            m_enabledLoggingKeys.erase(it);
            return true;
        }
        return false;
    }

    bool isKeyActive(const std::set<std::string>::iterator& loggingIterator) {
        return loggingIterator != m_enabledLoggingKeys.end();
    }

    template<typename... Args>
    void error(fmt::format_string<Args...> fmt, Args&&... args) {
        log(spdlog::level::err, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void error(const std::set<std::string>::iterator& it,
               fmt::format_string<Args...> fmt, Args&&... args) {
        if (isKeyActive(it))
            log(spdlog::level::err, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void info(fmt::format_string<Args...> fmt, Args&&... args) {
        log(spdlog::level::info, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void info(const std::set<std::string>::iterator& it,
              fmt::format_string<Args...> fmt, Args&&... args) {
        if (isKeyActive(it))
            log(spdlog::level::info, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void warn(fmt::format_string<Args...> fmt, Args&&... args) {
        log(spdlog::level::warn, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void warn(const std::set<std::string>::iterator& it,
              fmt::format_string<Args...> fmt, Args&&... args) {
        if (isKeyActive(it))
            log(spdlog::level::warn, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void critical(fmt::format_string<Args...> fmt, Args&&... args) {
        log(spdlog::level::critical, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void critical(const std::set<std::string>::iterator& it,
                  fmt::format_string<Args...> fmt, Args&&... args) {
        if (isKeyActive(it))
            log(spdlog::level::critical, fmt, std::forward<Args>(args)...);
    }

    void setLogLevelInfo(bool applyToFile = false) {
        setLevel(spdlog::level::info, applyToFile);
    }
    void setLogLevelError(bool applyToFile = false) {
        setLevel(spdlog::level::err, applyToFile);
    }
    void setLogLevelWarn(bool applyToFile = false) {
        setLevel(spdlog::level::warn, applyToFile);
    }
    void setLogLevelCritical(bool applyToFile = false) {
        setLevel(spdlog::level::critical, applyToFile);
    }
    void setLogLevelOff(bool applyToFile = false) {
        setLevel(spdlog::level::off, applyToFile);
    }

    // Explicit flush for async loggers
    void flush() {
        if (m_consoleLogger) m_consoleLogger->flush();
        if (m_enableFileLogging && m_fileLogger) m_fileLogger->flush();
    }

private:
    // Private Constructor (Runs once)
    AyonLogger() {
        std::cout << "[AyonLogger] Singleton Constructor Started." << std::endl;
        
        // Console logger (synchronous for immediate feedback)
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
        m_consoleLogger = std::make_shared<spdlog::logger>("console", console_sink);
        m_consoleLogger->set_level(spdlog::level::info);
        
        std::cout << "[AyonLogger] Singleton Constructor Finished." << std::endl;
    }

    ~AyonLogger() {
        // Flush and shutdown async loggers properly
        flush();
        spdlog::shutdown();
    }

    template<typename... Args>
    void log(spdlog::level::level_enum lvl,
             fmt::format_string<Args...> fmt,
             Args&&... args)
    {
        if (m_consoleLogger)
            m_consoleLogger->log(lvl, fmt, std::forward<Args>(args)...);

        if (m_enableFileLogging && m_fileLogger)
            m_fileLogger->log(lvl, fmt, std::forward<Args>(args)...);
    }

    void setLevel(spdlog::level::level_enum lvl, bool applyToFile) {
        if (m_consoleLogger)
            m_consoleLogger->set_level(lvl);
        if (applyToFile && m_fileLogger)
            m_fileLogger->set_level(lvl);
    }

    std::shared_ptr<spdlog::logger> m_consoleLogger;
    std::shared_ptr<spdlog::logger> m_fileLogger;

    bool m_enableFileLogging{false};
    std::set<std::string> m_enabledLoggingKeys;
};
