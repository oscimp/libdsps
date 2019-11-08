/* DSPS - library to build a digital signal processing simulation
 * Copyright (C) 2019  Arthur HUGEAT
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <dsps/Channel.h>
#include <dsps/Sum.h>

#include "local/Utils.h"

namespace {
    TEST(SumTest, testCompute2CH) {
        static constexpr unsigned N = 2048;

        // Alloc the task
        Sum<double> task(2);
        Channel in1;
        Channel in2;
        Channel &out = task.getOutput(0);

        // Connect input
        task.setInput(in1, 0);
        task.setInput(in2, 1);

        // Check if the channel was correct
        EXPECT_EQ(&task, in1.getOut());
        EXPECT_EQ(&task, in2.getOut());
        EXPECT_EQ(&task, out.getIn());

        // Open the oracle file
        std::ifstream fileIn1(std::string(ORACLE_DATA_DIR) + "/oracle_sum_input1.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileIn2(std::string(ORACLE_DATA_DIR) + "/oracle_sum_input2.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileOut(std::string(ORACLE_DATA_DIR) + "/oracle_sum_2ch.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(fileIn1.good());
        ASSERT_TRUE(fileIn2.good());
        ASSERT_TRUE(fileOut.good());

        // Test many loop
        for (std::size_t i = 0; i < 10; ++i) {
            // If no input data
            EXPECT_FALSE(task.isReady(N));

            // Send half data
            loadChannelFromOracleFile<double>(in1, fileIn1, N / 2);
            EXPECT_FALSE(task.isReady(N));
            loadChannelFromOracleFile<double>(in2, fileIn2, N / 2);
            EXPECT_FALSE(task.isReady(N));

            // Send all data
            loadChannelFromOracleFile<double>(in1, fileIn1, N / 2);
            EXPECT_FALSE(task.isReady(N));
            loadChannelFromOracleFile<double>(in2, fileIn2, N / 2);
            EXPECT_TRUE(task.isReady(N));
            EXPECT_FALSE(task.hasFinished(N));

            // Computing
            task.compute(N);
            EXPECT_FALSE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Compare the result
            auto result = extractVectorFromOracleFile<double>(fileOut, N);
            compareChannelWithVector(result, out, 0);
        }
    }

    TEST(SumTest, testCompute3CH) {
        static constexpr unsigned N = 2048;

        // Alloc the task
        Sum<double> task(3);
        Channel in1;
        Channel in2;
        Channel in3;
        Channel &out = task.getOutput(0);

        // Connect input
        task.setInput(in1, 0);
        task.setInput(in2, 1);
        task.setInput(in3, 2);

        // Check if the channel was correct
        EXPECT_EQ(&task, in1.getOut());
        EXPECT_EQ(&task, in2.getOut());
        EXPECT_EQ(&task, in3.getOut());
        EXPECT_EQ(&task, out.getIn());

        // Open the oracle file
        std::ifstream fileIn1(std::string(ORACLE_DATA_DIR) + "/oracle_sum_input1.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileIn2(std::string(ORACLE_DATA_DIR) + "/oracle_sum_input2.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileIn3(std::string(ORACLE_DATA_DIR) + "/oracle_sum_input3.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileOut(std::string(ORACLE_DATA_DIR) + "/oracle_sum_3ch.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(fileIn1.good());
        ASSERT_TRUE(fileIn2.good());
        ASSERT_TRUE(fileIn3.good());
        ASSERT_TRUE(fileOut.good());

        // Test many loop
        for (std::size_t i = 0; i < 10; ++i) {
            // If no input data
            EXPECT_FALSE(task.isReady(N));

            // Send half data
            loadChannelFromOracleFile<double>(in1, fileIn1, N / 2);
            EXPECT_FALSE(task.isReady(N));
            loadChannelFromOracleFile<double>(in2, fileIn2, N / 2);
            EXPECT_FALSE(task.isReady(N));
            loadChannelFromOracleFile<double>(in3, fileIn3, N / 2);
            EXPECT_FALSE(task.isReady(N));

            // Send all data
            loadChannelFromOracleFile<double>(in2, fileIn2, N / 2);
            EXPECT_FALSE(task.isReady(N));
            loadChannelFromOracleFile<double>(in3, fileIn3, N / 2);
            EXPECT_FALSE(task.isReady(N));
            loadChannelFromOracleFile<double>(in1, fileIn1, N / 2);
            EXPECT_TRUE(task.isReady(N));
            EXPECT_FALSE(task.hasFinished(N));

            // Computing
            task.compute(N);
            EXPECT_FALSE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Compare the result
            auto result = extractVectorFromOracleFile<double>(fileOut, N);
            compareChannelWithVector(result, out, 0);
        }
    }

    TEST(SumTest, testCompute5CH) {
        static constexpr unsigned N = 2048;

        // Alloc the task
        Sum<double> task(5);
        Channel in1;
        Channel in2;
        Channel in3;
        Channel in4;
        Channel in5;
        Channel &out = task.getOutput(0);

        // Connect input
        task.setInput(in1, 0);
        task.setInput(in2, 1);
        task.setInput(in3, 2);
        task.setInput(in4, 3);
        task.setInput(in5, 4);

        // Check if the channel was correct
        EXPECT_EQ(&task, in1.getOut());
        EXPECT_EQ(&task, in2.getOut());
        EXPECT_EQ(&task, in3.getOut());
        EXPECT_EQ(&task, in4.getOut());
        EXPECT_EQ(&task, in5.getOut());
        EXPECT_EQ(&task, out.getIn());

        // Open the oracle file
        std::ifstream fileIn1(std::string(ORACLE_DATA_DIR) + "/oracle_sum_input1.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileIn2(std::string(ORACLE_DATA_DIR) + "/oracle_sum_input2.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileIn3(std::string(ORACLE_DATA_DIR) + "/oracle_sum_input3.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileIn4(std::string(ORACLE_DATA_DIR) + "/oracle_sum_input4.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileIn5(std::string(ORACLE_DATA_DIR) + "/oracle_sum_input5.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileOut(std::string(ORACLE_DATA_DIR) + "/oracle_sum_5ch.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(fileIn1.good());
        ASSERT_TRUE(fileIn2.good());
        ASSERT_TRUE(fileIn3.good());
        ASSERT_TRUE(fileIn4.good());
        ASSERT_TRUE(fileIn5.good());
        ASSERT_TRUE(fileOut.good());

        // Test many loop
        for (std::size_t i = 0; i < 10; ++i) {
            // If no input data
            EXPECT_FALSE(task.isReady(N));

            // Send half data
            loadChannelFromOracleFile<double>(in1, fileIn1, N / 2);
            EXPECT_FALSE(task.isReady(N));
            loadChannelFromOracleFile<double>(in2, fileIn2, N / 2);
            EXPECT_FALSE(task.isReady(N));
            loadChannelFromOracleFile<double>(in3, fileIn3, N / 2);
            EXPECT_FALSE(task.isReady(N));
            loadChannelFromOracleFile<double>(in4, fileIn4, N / 2);
            EXPECT_FALSE(task.isReady(N));
            loadChannelFromOracleFile<double>(in5, fileIn5, N / 2);
            EXPECT_FALSE(task.isReady(N));

            // Send all data
            loadChannelFromOracleFile<double>(in1, fileIn1, N / 2);
            EXPECT_FALSE(task.isReady(N));
            loadChannelFromOracleFile<double>(in2, fileIn2, N / 2);
            EXPECT_FALSE(task.isReady(N));
            loadChannelFromOracleFile<double>(in3, fileIn3, N / 2);
            EXPECT_FALSE(task.isReady(N));
            loadChannelFromOracleFile<double>(in5, fileIn5, N / 2);
            EXPECT_FALSE(task.isReady(N));
            loadChannelFromOracleFile<double>(in4, fileIn4, N / 2);
            EXPECT_TRUE(task.isReady(N));
            EXPECT_FALSE(task.hasFinished(N));

            // Computing
            task.compute(N);
            EXPECT_FALSE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Compare the result
            auto result = extractVectorFromOracleFile<double>(fileOut, N);
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
