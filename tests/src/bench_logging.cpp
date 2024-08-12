#include "AyonLogger.h"
#include <benchmark/benchmark.h>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>

std::unique_ptr<AyonLogger> TestLogger;
bool falseBool = false;
bool trueBool = true;

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
        TestLogger->regesterLoggingKey("TestKeyA");
    }
}
BENCHMARK(BM_AyonLoggerRegKey);

void
BM_AyonLoggerUnRegKey(benchmark::State &state) {
    for (auto _: state) {
        TestLogger->unregisterLoggingKey("TestKeyA");
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
        TestLogger->info(TestLogger->key("KeyA"), "TestA");
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

static void
BM_CompareNumbersRefBench(benchmark::State &state) {
    int a = state.range(0);
    int b = state.range(1);
    bool result;

    for (auto _: state) {
        benchmark::DoNotOptimize(a);
        benchmark::DoNotOptimize(b);
        result = (a < b);
        benchmark::DoNotOptimize(result);
    }
}

// Register the benchmark with a range of values
BENCHMARK(BM_CompareNumbersRefBench)->Ranges({{1, 2}, {1, 3}});

int
main(int argc, char** argv) {
    TestLogger
        = std::make_unique<AyonLogger>(AyonLogger::getInstance("/home/lyonh/ynput/DevEnv/projects/AyonCppLogger/"
                                                               "AyonCppLogger/test/out/test.json"));

    TestLogger->regesterLoggingKey("ActiveKey");

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}
