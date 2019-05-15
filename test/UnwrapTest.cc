#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <dsps/Channel.h>
#include <dsps/Unwrap.h>

#include "local/Utils.h"

namespace {
    TEST(UnwrapTest, testCompute1Block) {
        static constexpr unsigned N = 20480;

        // Alloc the task
        Unwrap task;
        Channel in;
        Channel &out = task.getOutput(0);

        // Connect input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());
        EXPECT_EQ(&task, out.getIn());

        // Open the oracle file
        std::ifstream fileIn(std::string(ORACLE_DATA_DIR) + "/oracle_unwrap_input.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileOut(std::string(ORACLE_DATA_DIR) + "/oracle_unwrap_output_1_bloc.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(fileIn.good());
        ASSERT_TRUE(fileOut.good());

        // If no input data
        EXPECT_FALSE(task.isReady(N));

        // Send half data
        loadChannelFromOracleFile<double>(in, fileIn, N/2);
        EXPECT_FALSE(task.isReady(N));

        // Send all data
        loadChannelFromOracleFile<double>(in, fileIn, N/2);
        EXPECT_TRUE(task.isReady(N));
        EXPECT_FALSE(task.hasFinished(N));

        // Computing
        task.compute(N);
        EXPECT_FALSE(task.isReady(N));
        EXPECT_TRUE(task.hasFinished(N));

        // Compare the result
        std::vector<double> result = extractVectorFromOracleFile<double>(fileOut, N);
        compareChannelWithVector(result, out, 0.0);
    }

    TEST(UnwrapTest, testCompute10Block) {
        static constexpr unsigned N = 2048;

        // Alloc the task
        Unwrap task;
        Channel in;
        Channel &out = task.getOutput(0);

        // Connect input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());
        EXPECT_EQ(&task, out.getIn());

        // Open the oracle file
        std::ifstream fileIn(std::string(ORACLE_DATA_DIR) + "/oracle_unwrap_input.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileOut(std::string(ORACLE_DATA_DIR) + "/oracle_unwrap_output_10_bloc.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(fileIn.good());
        ASSERT_TRUE(fileOut.good());

        for (std::size_t i = 0; i < 10; ++i) {
            // If no input data
            EXPECT_FALSE(task.isReady(N));

            // Send half data
            loadChannelFromOracleFile<double>(in, fileIn, N/2);
            EXPECT_FALSE(task.isReady(N));

            // Send all data
            loadChannelFromOracleFile<double>(in, fileIn, N/2);
            EXPECT_TRUE(task.isReady(N));
            EXPECT_FALSE(task.hasFinished(N));

            // Computing
            task.compute(N);
            EXPECT_FALSE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Compare the result
            std::vector<double> result = extractVectorFromOracleFile<double>(fileOut, N);
            compareChannelWithVector(result, out, 0.0);
        }
    }
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);
    ::testing::GTEST_FLAG(throw_on_failure) = true;
    ::testing::FLAGS_gtest_death_test_style = "fast";
    return RUN_ALL_TESTS();
}
