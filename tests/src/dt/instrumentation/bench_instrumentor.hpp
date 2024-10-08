#include <benchmark/benchmark.h>
#include <string>
#include "ynput/dt/instrumentation/InstrumentorMacros.h"

void
BM_AyonInstrumenterBeginSession(benchmark::State &state) {
    for (auto _: state) {
        std::string traceFile = Instrumentor::Get().BeginSession();
        state.PauseTiming();
        if (std::filesystem::exists(traceFile)) {
            std::filesystem::remove(traceFile);
        }
        state.ResumeTiming();
    }
}
BENCHMARK(BM_AyonInstrumenterBeginSession);

void
BM_AyonInstrumenterTraceFunc(benchmark::State &state) {
    std::string traceFile = Instrumentor::Get().BeginSession();

    for (auto _: state) {
        InstrumentFunc("Name", "GRP", {})
    }

    if (std::filesystem::exists(traceFile)) {
        std::filesystem::remove(traceFile);
    }
}
BENCHMARK(BM_AyonInstrumenterTraceFunc);
