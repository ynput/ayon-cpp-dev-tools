#include <benchmark/benchmark.h>
#include "dt/instrumentation/bench_instrumentor.hpp"

#ifdef AYON_CPPTOOLS_BUILD_LOGGER
    #include "lib/logging/bench_logger.hpp"

#endif   // AYON_CPPTOOLS_BUILD_LOGGER

int
main(int argc, char** argv) {
#ifdef AYON_CPPTOOLS_BUILD_LOGGER
    setupLoggerBench();
#endif

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}
