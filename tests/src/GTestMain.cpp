#include "gtest/gtest.h"
#include "lib/logging/test_logger.hpp"

int
main(int argc, char** argv) {
    // Run Setup functions for tests
    setupLoggerTests();

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
