#include "AyonLogger.h"
#include "gtest/gtest.h"
#include <memory>
#include <string>

std::unique_ptr<AyonLogger> TestLoggerGlob;

TEST(AyonLogger, AyonLoggerCreation) {
    AyonLogger TestLogger = AyonLogger::getInstance("test/LogFile.json");
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

TEST(AyonLogger, AyonLoggerLogAllNoKey) {
    std::unique_ptr<AyonLogger> TestLogger;

    TestLogger = std::make_unique<AyonLogger>(AyonLogger::getInstance("test/LogFile.json"));

    TestLogger->info("Info Print Test No Key");
    TestLogger->warn("Warn Print Test No Key");
    TestLogger->error("Error Print Test No Key");
    TestLogger->critical("Critical Print Test No Key");
}

TEST(AyonLogger, AyonLoggerLogAllKey) {
    std::unique_ptr<AyonLogger> TestLogger;

    TestLogger = std::make_unique<AyonLogger>(AyonLogger::getInstance("test/LogFile.json"));

    TestLogger->regesterLoggingKey("KeyA");

    TestLogger->info(TestLogger->key("KeyA"), "Info Print Test With Key");
    TestLogger->warn(TestLogger->key("KeyA"), "Warn Print Test With Key");
    TestLogger->error(TestLogger->key("KeyA"), "Error Print Test With Key");
    TestLogger->critical(TestLogger->key("KeyA"), "Critical Print Test With Key");
}

TEST(AyonLogger, AyonLoggerLogAllKeyWrongKeyReg) {
    std::unique_ptr<AyonLogger> TestLogger;

    TestLogger = std::make_unique<AyonLogger>(AyonLogger::getInstance("test/LogFile.json"));

    TestLogger->regesterLoggingKey("KeyB");

    TestLogger->info(TestLogger->key("KeyA"), "Info Print Test With Key");
    TestLogger->warn(TestLogger->key("KeyA"), "Warn Print Test With Key");
    TestLogger->error(TestLogger->key("KeyA"), "Error Print Test With Key");
    TestLogger->critical(TestLogger->key("KeyA"), "Critical Print Test With Key");
}

int
main(int argc, char** argv) {
    TestLoggerGlob = std::make_unique<AyonLogger>(
        AyonLogger::getInstance(std::filesystem::current_path().string() + "/test/out/LogBench.json"));

    TestLoggerGlob->regesterLoggingKey("RegKeyC");

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
