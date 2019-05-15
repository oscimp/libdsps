#include <cmath>
#include <complex>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <dsps/Atan2.h>
#include <dsps/Channel.h>

#include "local/Utils.h"

using ::testing::Return;

namespace {
    TEST(Atan2Test, testCompute2Over1) {
        // Declare the mock channels
        static constexpr unsigned N = 2048;

        // Alloc the task
        Atan2 task;
        Channel i;
        Channel q;
        Channel &out = task.getOutput(0);

        // Connect input
        task.setInput(q, 0);
        task.setInput(i, 1);

        // Check if the channel was correct
        EXPECT_EQ(&task, i.getOut());
        EXPECT_EQ(&task, q.getOut());
        EXPECT_EQ(&task, out.getIn());

        // Open the oracle file
        std::ifstream fileI(std::string(ORACLE_DATA_DIR) + "/oracle_atan2_input1.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileQ(std::string(ORACLE_DATA_DIR) + "/oracle_atan2_input2.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileOut(std::string(ORACLE_DATA_DIR) + "/oracle_atan2_output_2_over_1.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(fileI.good());
        ASSERT_TRUE(fileQ.good());
        ASSERT_TRUE(fileOut.good());

        // If no input data
        EXPECT_FALSE(task.isReady(N));

        for (std::size_t j = 0; j < 10; ++j) {
            // Send half data
            loadChannelFromOracleFile<double>(i, fileI, N/2);
            loadChannelFromOracleFile<double>(q, fileQ, N/2);
            EXPECT_FALSE(task.isReady(N));

            // Send all data
            loadChannelFromOracleFile<double>(i, fileI, N/2);
            loadChannelFromOracleFile<double>(q, fileQ, N/2);
            EXPECT_TRUE(task.isReady(N));
            EXPECT_FALSE(task.hasFinished(N));

            // Computing
            task.compute(N);
            EXPECT_FALSE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Compare the result
            std::vector<double> result = extractVectorFromOracleFile<double>(fileOut, N);
            compareChannelWithVector(result, out, 0);
        }
    }

    TEST(Atan2Test, testCompute1Over2) {
        // Declare the mock channels
        static constexpr unsigned N = 2048;

        // Alloc the task
        Atan2 task;
        Channel i;
        Channel q;
        Channel &out = task.getOutput(0);

        // Connect input
        task.setInput(q, 0);
        task.setInput(i, 1);

        // Check if the channel was correct
        EXPECT_EQ(&task, i.getOut());
        EXPECT_EQ(&task, q.getOut());
        EXPECT_EQ(&task, out.getIn());

        // Open the oracle file
        std::ifstream fileI(std::string(ORACLE_DATA_DIR) + "/oracle_atan2_input2.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileQ(std::string(ORACLE_DATA_DIR) + "/oracle_atan2_input1.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileOut(std::string(ORACLE_DATA_DIR) + "/oracle_atan2_output_1_over_2.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(fileI.good());
        ASSERT_TRUE(fileQ.good());
        ASSERT_TRUE(fileOut.good());

        // If no input data
        EXPECT_FALSE(task.isReady(N));

        for (std::size_t j = 0; j < 10; ++j) {
            // Send half data
            loadChannelFromOracleFile<double>(i, fileI, N/2);
            loadChannelFromOracleFile<double>(q, fileQ, N/2);
            EXPECT_FALSE(task.isReady(N));

            // Send all data
            loadChannelFromOracleFile<double>(i, fileI, N/2);
            loadChannelFromOracleFile<double>(q, fileQ, N/2);
            EXPECT_TRUE(task.isReady(N));
            EXPECT_FALSE(task.hasFinished(N));

            // Computing
            task.compute(N);
            EXPECT_FALSE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Compare the result
            std::vector<double> result = extractVectorFromOracleFile<double>(fileOut, N);
            compareChannelWithVector(result, out, 0);
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
