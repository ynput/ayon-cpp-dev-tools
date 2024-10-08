#include <charconv>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <thread>
#include "ynput/dt/instrumentation/InstrumentorMacros.h"
#include "gtest/gtest.h"

std::string
getTraceFileData(const std::string &traceFile) {
    std::ifstream file(traceFile);
    if (!file.is_open()) {
        throw std::invalid_argument("Can't open Trace File: " + traceFile);
    }

    std::stringstream traceFileBuffer;
    traceFileBuffer << file.rdbuf();

    try {
        auto size = std::filesystem::file_size(traceFile);
    }
    catch (const std::filesystem::filesystem_error &e) {
        throw std::invalid_argument("Can't get size or open the trace file: " + traceFile);
    }
    return traceFileBuffer.str();
}

class InstrumentFixture: public ::testing::Test {
    public:
        std::string m_traceFile;

    protected:
        void
        SetUp() override {
            m_traceFile = Instrumentor::Get().BeginSession();
        }

        void
        TearDown() override {
            if (std::filesystem::exists(m_traceFile)) {
                std::filesystem::remove(m_traceFile);
            }
        }
};

std::string
simpleRandomeStr() {
    std::chrono::milliseconds ms
        = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

    std::ostringstream oss;
    oss << ms.count();

    return "rand_" + oss.str() + "_data";
}

int
func1() {
    InstrumentFunc("Func1", "LayerdFunc", {{"testa", "test1"}});
    std::srand(std::time(nullptr));
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    return std::rand();
}
int
func2() {
    InstrumentFunc("Func2", "LayerdFunc", {{"testb", "test2"}});
    std::srand(std::time(nullptr));
    return func1() + std::rand();
}
int
func3() {
    InstrumentFunc("Func3", "LayerdFunc", {{"testc", "test3"}});
    std::srand(std::time(nullptr));
    return func2() + std::rand();
}

int
funcRecu(int depth) {
    std::string name = "RecuFunc_" + std::to_string(depth);
    InstrumentFunc(name.c_str(), "LayerdFunc", {});
    if (depth > 10) {
        return 0;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    depth = depth + funcRecu(depth + 1);
    return depth;
}

TEST_F(InstrumentFixture, InstrumentFuncStack) {
    func3();
    std::string data = getTraceFileData(m_traceFile);

    ASSERT_TRUE(data.find("Func3") != std::string::npos) << "Wrong data or no data in trace file";
}

TEST_F(InstrumentFixture, InstrumentFuncRecursive) {
    funcRecu(0);

    std::string data = getTraceFileData(m_traceFile);
    for (int i = 0; i < 10; ++i) {
        ASSERT_TRUE(data.find("RecuFunc_" + std::to_string(i)) != std::string::npos)
            << "Wrong data or no data in trace file";
    }
}

TEST_F(InstrumentFixture, InstrumentSimpleScopeWith1MsSleep) {
    {
        InstrumentFunc("ScopedFuncTest", "Scope", {{"test", "test"}});
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::string data = getTraceFileData(m_traceFile);
    ASSERT_TRUE(data.find("ScopedFuncTest") != std::string::npos) << "Wrong data or no data in trace file";
}

void
asyncTask(int id) {
    std::string name = "AsyncFunc" + std::to_string(id);
    InstrumentFunc(name.c_str(), "LayerdFunc", {{"testc", "1"}});
    std::srand(std::time(nullptr));
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

TEST_F(InstrumentFixture, InstrumentAsync) {
    std::vector<std::future<void>> futures;
    int range(100);
    for (int i = 0; i < range; ++i) {
        futures.push_back(std::async(std::launch::async, asyncTask, i));
    }
    for (auto &fut: futures) {
        fut.get();
    }

    std::string data = getTraceFileData(m_traceFile);
    for (int i = 0; i < range; ++i) {
        ASSERT_TRUE(data.find("AsyncFunc" + std::to_string(i)) != std::string::npos)
            << "Wrong data or no data in trace file";
    }
}
