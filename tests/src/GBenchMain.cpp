#include "lib/logging/bench_logger.hpp"

int
main(int argc, char** argv) {
    setupLoggerBench();

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}
