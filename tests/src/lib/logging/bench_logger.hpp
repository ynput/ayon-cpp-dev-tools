#include "ynput/lib/logging/AyonLogger.hpp"
#include <benchmark/benchmark.h>

#include <memory>
#include <string>

std::unique_ptr<AyonLogger> TestLogger;

void
BM_AyonLoggerConstruction(benchmark::State &state) {
    for (auto _: state) {
        AyonLogger Log = AyonLogger::getInstance("Test");
    }
}
BENCHMARK(BM_AyonLoggerConstruction);

void
BM_AyonLoggerLogInfoNoKey(benchmark::State &state) {
    for (auto _: state) {
        TestLogger->info("TestA");
    }
}
BENCHMARK(BM_AyonLoggerLogInfoNoKey);

void
BM_AyonLoggerRegKey(benchmark::State &state) {
    for (auto _: state) {
        auto start = std::chrono::high_resolution_clock::now();

        TestLogger->regesterLoggingKey("TestKeyA");

        auto end = std::chrono::high_resolution_clock::now();

        TestLogger->unregisterLoggingKey("TestKeyA");

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        benchmark::DoNotOptimize(duration);
    }
}
BENCHMARK(BM_AyonLoggerRegKey);

void
BM_AyonLoggerUnRegKey(benchmark::State &state) {
    for (auto _: state) {
        TestLogger->regesterLoggingKey("TestKeyA");
        auto start = std::chrono::high_resolution_clock::now();

        TestLogger->unregisterLoggingKey("TestKeyA");

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        benchmark::DoNotOptimize(duration);
    }
}
BENCHMARK(BM_AyonLoggerUnRegKey);

void
BM_AyonLoggerRegUnRegKey(benchmark::State &state) {
    for (auto _: state) {
        TestLogger->regesterLoggingKey("TestKeyB");
        TestLogger->unregisterLoggingKey("TestKeyB");
    }
}
BENCHMARK(BM_AyonLoggerRegUnRegKey);

void
BM_AyonLoggerLogInfoWrongKey(benchmark::State &state) {
    for (auto _: state) {
        TestLogger->info(TestLogger->key("WrongKey"), "TestA");
    }
}
BENCHMARK(BM_AyonLoggerLogInfoWrongKey);

void
BM_AyonLoggerLogInfoRightKey(benchmark::State &state) {
    for (auto _: state) {
        TestLogger->info(TestLogger->key("ActiveKey"), "TestA");
    }
}
BENCHMARK(BM_AyonLoggerLogInfoRightKey);

void
BM_AyonLoggerIsKeyActiveTrue(benchmark::State &state) {
    for (auto _: state) {
        if (TestLogger->isKeyActive(TestLogger->key("ActiveKey"))) {
        }
    }
}
BENCHMARK(BM_AyonLoggerIsKeyActiveTrue);

void
BM_AyonLoggerIsKeyActiveFalse(benchmark::State &state) {
    for (auto _: state) {
        if (TestLogger->isKeyActive(TestLogger->key("NonActiveKey"))) {
        }
    }
}
BENCHMARK(BM_AyonLoggerIsKeyActiveFalse);

void
setupLoggerBench() {
    TestLogger = std::make_unique<AyonLogger>(
        AyonLogger::getInstance(std::filesystem::current_path().string() + "/tests/out/LogTest.json"));

    TestLogger->regesterLoggingKey("ActiveKey");
}
