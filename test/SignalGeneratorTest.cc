#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <dsps/Channel.h>
#include <dsps/SignalGenerator.h>

#include "local/Utils.h"

namespace {
    TEST(SignalGeneratorTest, testComputeReadyPrettyFsFc) {
        const unsigned N = 2048;
        const unsigned n = 1000 * N;
        const double AMPLITUDE = 2.5;
        const double FC = 10e6;
        const double FS = 250e6;

        // Alloc the task
        SignalGenerator task(AMPLITUDE, FC, FS);
        Channel &out = task.getOutput(0);

        // Check if the channel was correct
        EXPECT_EQ(&task, out.getIn());

        // The task is always ready
        EXPECT_TRUE(task.isReady(0));
        EXPECT_TRUE(task.isReady(N));
        EXPECT_TRUE(task.isReady(n));
        EXPECT_TRUE(task.isReady(N / 2));

        // Open the oracle file
        std::ifstream file(std::string(ORACLE_DATA_DIR) + "/oracle_signal_pretty_output.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(file.good());

        for (std::size_t i = 0; i < 1000; ++i) {
            // Computing
            EXPECT_TRUE(task.isReady(N));
            EXPECT_FALSE(task.hasFinished(N));
            task.compute(N);
            EXPECT_TRUE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Compare the result
            auto vec = extractVectorFromOracleFile<double>(file, N);
            compareChannelWithVector(vec, out, 1e-9);
        }
    }

    TEST(SignalGeneratorTest, testComputeReadyUglyFsFc) {
        const unsigned N = 2048;
        const unsigned n = 1000 * N;
        const double AMPLITUDE = 1.25;
        const double FC = 10e6 + 5e3;
        const double FS = 125e6;

        // Alloc the task
        SignalGenerator task(AMPLITUDE, FC, FS);
        Channel &out = task.getOutput(0);

        // Check if the channel was correct
        EXPECT_EQ(&task, out.getIn());

        // The task is always ready
        EXPECT_TRUE(task.isReady(0));
        EXPECT_TRUE(task.isReady(N));
        EXPECT_TRUE(task.isReady(n));
        EXPECT_TRUE(task.isReady(N / 2));

        // Open the oracle file
        std::ifstream file(std::string(ORACLE_DATA_DIR) + "/oracle_signal_ugly_output.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(file.good());

        for (std::size_t i = 0; i < 1000; ++i) {
            // Computing
            EXPECT_TRUE(task.isReady(N));
            EXPECT_FALSE(task.hasFinished(N));
            task.compute(N);
            EXPECT_TRUE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Compare the result
            auto vec = extractVectorFromOracleFile<double>(file, N);
            compareChannelWithVector(vec, out, 1e-9);
        }
    }

    TEST(SignalGeneratorTest, testComputeReadyVeryUglyFsFc) {
        const unsigned N = 2048;
        const unsigned n = 1000 * N;
        const double AMPLITUDE = 0.25;
        const double FC = 897.254e4 + 5.98e2;
        const double FS = 79.5e6;

        // Alloc the task
        SignalGenerator task(AMPLITUDE, FC, FS);
        Channel &out = task.getOutput(0);

        // Check if the channel was correct
        EXPECT_EQ(&task, out.getIn());

        // The task is always ready
        EXPECT_TRUE(task.isReady(0));
        EXPECT_TRUE(task.isReady(N));
        EXPECT_TRUE(task.isReady(n));
        EXPECT_TRUE(task.isReady(N / 2));

        // Open the oracle file
        std::ifstream file(std::string(ORACLE_DATA_DIR) + "/oracle_signal_very_ugly_output.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(file.good());

        for (std::size_t i = 0; i < 1000; ++i) {
            // Computing
            EXPECT_TRUE(task.isReady(N));
            EXPECT_FALSE(task.hasFinished(N));
            task.compute(N);
            EXPECT_TRUE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Compare the result
            auto vec = extractVectorFromOracleFile<double>(file, N);
            compareChannelWithVector(vec, out, 1e-9);
        }
    }

    /// FIXIT: Death
    // TEST(SignalGeneratorTest, testComputeReadyFailTooMemory) {
    //     static constexpr double AMPLITUDE = 0.525;
    //     static constexpr double FC = 87.3974e4 + 3.5877e2;
    //     static constexpr double FS = 578.31458e6;

    //     // Alloc the task
    //     Channel<double> out;

    //     // ASSERT_DEATH({ SignalGenerator task(out, AMPLITUDE, FC, FS); }, "SignalGenerator::findCorrectLUTSize(): Too memory allocated!");
    //     ASSERT_EXIT({ SignalGenerator task(out, AMPLITUDE, FC, FS); }, ::testing::ExitedWithCode(-1), "SignalGenerator::findCorrectLUTSize(): Too memory allocated!");

    //     // EXPECT_DEATH({ SignalGenerator task(out, AMPLITUDE, FC, FS); }, "SignalGenerator::findCorrectLUTSize(): Too memory allocated!");
    // }
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);
    ::testing::GTEST_FLAG(throw_on_failure) = true;
    ::testing::FLAGS_gtest_death_test_style = "fast";
    return RUN_ALL_TESTS();
}
