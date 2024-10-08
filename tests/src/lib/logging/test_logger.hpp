#include "ynput/lib/logging/AyonLogger.hpp"
#include "gtest/gtest.h"
#include <string>

std::unique_ptr<AyonLogger> TestLoggerGlob;

TEST(AyonLogger, AyonLoggerCreation) {
    AyonLogger TestLogger = AyonLogger::getInstance("tests/LogFile.json");
}

TEST(AyonLogger, AyonLoggerRegKey) {
    TestLoggerGlob->regesterLoggingKey("RegKey");
}

TEST(AyonLogger, AyonLoggerUnRegKey) {
    TestLoggerGlob->unregisterLoggingKey("RegKey");
}

TEST(AyonLogger, AyonLoggerRegUnRegKey) {
    TestLoggerGlob->regesterLoggingKey("RegKeyB");
    TestLoggerGlob->unregisterLoggingKey("RegKeyB");
}

TEST(AyonLogger, AyonLoggerIsKeyActiveTrue) {
    if (TestLoggerGlob->isKeyActive(TestLoggerGlob->key("RegKeyC"))) {
    }
}

TEST(AyonLogger, AyonLoggerIsKeyActiveFalse) {
    if (TestLoggerGlob->isKeyActive(TestLoggerGlob->key("RegKeyZ"))) {
    }
}

TEST(AyonLogger, AyonLoggerLogInfoNoKey) {
    std::unique_ptr<AyonLogger> TestLogger;

    TestLogger = std::make_unique<AyonLogger>(AyonLogger::getInstance("tests/LogFile.json"));

    TestLogger->info("Info Print Test No Key");

    std::string test = AyonTestOss.str();
    EXPECT_TRUE(test.find("Info Print Test No Key") != std::string::npos);
    AyonTestOss.str("");
}

TEST(AyonLogger, AyonLoggerLogWarnNoKey) {
    std::unique_ptr<AyonLogger> TestLogger;

    TestLogger = std::make_unique<AyonLogger>(AyonLogger::getInstance("tests/LogFile.json"));

    TestLogger->warn("Warn Print Test No Key");

    std::string test = AyonTestOss.str();
    EXPECT_TRUE(test.find("Warn Print Test No Key") != std::string::npos);
    AyonTestOss.str("");
}

TEST(AyonLogger, AyonLoggerLogErrNoKey) {
    std::unique_ptr<AyonLogger> TestLogger;

    TestLogger = std::make_unique<AyonLogger>(AyonLogger::getInstance("tests/LogFile.json"));

    TestLogger->error("Error Print Test No Key");

    std::string test = AyonTestOss.str();
    EXPECT_TRUE(test.find("Error Print Test No Key") != std::string::npos);
    AyonTestOss.str("");
}

TEST(AyonLogger, AyonLoggerLogCritNoKey) {
    std::unique_ptr<AyonLogger> TestLogger;

    TestLogger = std::make_unique<AyonLogger>(AyonLogger::getInstance("tests/LogFile.json"));

    TestLogger->critical("Critical Print Test No Key");

    std::string test = AyonTestOss.str();
    EXPECT_TRUE(test.find("Critical Print Test No Key") != std::string::npos);
    AyonTestOss.str("");
}

TEST(AyonLogger, AyonLoggerLogInfoKey) {
    std::unique_ptr<AyonLogger> TestLogger;

    TestLogger = std::make_unique<AyonLogger>(AyonLogger::getInstance("tests/LogFile.json"));

    TestLogger->regesterLoggingKey("KeyA");

    TestLogger->info(TestLogger->key("KeyA"), "Info Print Test With Key");

    std::string test = AyonTestOss.str();
    EXPECT_TRUE(test.find("Info Print Test With Key") != std::string::npos);
    AyonTestOss.str("");
}
TEST(AyonLogger, AyonLoggerLogWarnKey) {
    std::unique_ptr<AyonLogger> TestLogger;

    TestLogger = std::make_unique<AyonLogger>(AyonLogger::getInstance("tests/LogFile.json"));

    TestLogger->regesterLoggingKey("KeyA");

    TestLogger->warn(TestLogger->key("KeyA"), "Warn Print Test With Key");

    std::string test = AyonTestOss.str();
    EXPECT_TRUE(test.find("Warn Print Test With Key") != std::string::npos);
    AyonTestOss.str("");
}
TEST(AyonLogger, AyonLoggerLogErrKey) {
    std::unique_ptr<AyonLogger> TestLogger;

    TestLogger = std::make_unique<AyonLogger>(AyonLogger::getInstance("tests/LogFile.json"));

    TestLogger->regesterLoggingKey("KeyA");

    TestLogger->error(TestLogger->key("KeyA"), "Error Print Test With Key");
    std::string test = AyonTestOss.str();
    EXPECT_TRUE(test.find("Error Print Test With Key") != std::string::npos);
    AyonTestOss.str("");
}
TEST(AyonLogger, AyonLoggerLogCritKey) {
    std::unique_ptr<AyonLogger> TestLogger;

    TestLogger = std::make_unique<AyonLogger>(AyonLogger::getInstance("tests/LogFile.json"));

    TestLogger->regesterLoggingKey("KeyA");

    TestLogger->critical(TestLogger->key("KeyA"), "Critical Print Test With Key");
    std::string test = AyonTestOss.str();
    EXPECT_TRUE(test.find("Critical Print Test With Key") != std::string::npos);
    AyonTestOss.str("");
}

TEST(AyonLogger, AyonLoggerLogInfoKeyWrongKeyReg) {
    std::unique_ptr<AyonLogger> TestLogger;

    TestLogger = std::make_unique<AyonLogger>(AyonLogger::getInstance("tests/LogFile.json"));

    TestLogger->regesterLoggingKey("KeyB");

    TestLogger->info(TestLogger->key("KeyA"), "Info Print Test With Key");

    std::string test = AyonTestOss.str();
    EXPECT_FALSE(test.find("Info Print Test With Key") != std::string::npos);
    AyonTestOss.str("");
}
TEST(AyonLogger, AyonLoggerLogWarnKeyWrongKeyReg) {
    std::unique_ptr<AyonLogger> TestLogger;

    TestLogger = std::make_unique<AyonLogger>(AyonLogger::getInstance("tests/LogFile.json"));

    TestLogger->regesterLoggingKey("KeyB");

    TestLogger->warn(TestLogger->key("KeyA"), "Warn Print Test With Key");
    std::string test = AyonTestOss.str();
    EXPECT_FALSE(test.find("Warn Print Test With Key") != std::string::npos);
    AyonTestOss.str("");
}
TEST(AyonLogger, AyonLoggerLogErrKeyWrongKeyReg) {
    std::unique_ptr<AyonLogger> TestLogger;

    TestLogger = std::make_unique<AyonLogger>(AyonLogger::getInstance("tests/LogFile.json"));

    TestLogger->regesterLoggingKey("KeyB");

    TestLogger->error(TestLogger->key("KeyA"), "Error Print Test With Key");
    std::string test = AyonTestOss.str();
    EXPECT_FALSE(test.find("Error Print Test With Key") != std::string::npos);
    AyonTestOss.str("");
}
TEST(AyonLogger, AyonLoggerLogCritKeyWrongKeyReg) {
    std::unique_ptr<AyonLogger> TestLogger;

    TestLogger = std::make_unique<AyonLogger>(AyonLogger::getInstance("tests/LogFile.json"));

    TestLogger->regesterLoggingKey("KeyB");

    TestLogger->critical(TestLogger->key("KeyA"), "Critical Print Test With Key");
    std::string test = AyonTestOss.str();
    EXPECT_FALSE(test.find("Critical Print Test With Key") != std::string::npos);
    AyonTestOss.str("");
}

void
setupLoggerTests() {
    TestLoggerGlob = std::make_unique<AyonLogger>(
        AyonLogger::getInstance(std::filesystem::current_path().string() + "/tests/out/LogTest.json"));

    TestLoggerGlob->regesterLoggingKey("RegKeyC");
}
