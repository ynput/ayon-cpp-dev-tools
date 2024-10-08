#include "gtest/gtest.h"
#include "dt/instrumentation/test_instrumentor.hpp"

#ifdef AYON_CPPTOOLS_BUILD_LOGGER
    #include "lib/logging/test_logger.hpp"
#endif   // AYON_CPPTOOLS_BUILD_LOGGER

int
main(int argc, char** argv) {
#ifdef AYON_CPPTOOLS_BUILD_LOGGER
    setupLoggerTests();
#endif

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
