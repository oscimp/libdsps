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

#include <cmath>
#include <complex>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <dsps/Channel.h>
#include <dsps/Demodulation.h>

#include "local/Utils.h"

namespace {
    TEST(DemodulationTest, testComputePrettyFsFcWithoutOffset) {
        static constexpr unsigned N = 2048;

        // Alloc the task
        Demodulation task(10e6, 250e6, 0.0);
        Channel in;
        Channel &ibf = task.getOutput(0);
        Channel &qbf = task.getOutput(1);

        // Connect the input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());
        EXPECT_EQ(&task, ibf.getIn());
        EXPECT_EQ(&task, qbf.getIn());

        // Open the oracle file
        std::ifstream fileIn(std::string(ORACLE_DATA_DIR) + "/oracle_demodulation_pretty_input.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileIbf(std::string(ORACLE_DATA_DIR) + "/oracle_demodulation_pretty_wo_offset_ibf.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileQbf(std::string(ORACLE_DATA_DIR) + "/oracle_demodulation_pretty_wo_offset_qbf.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(fileIn.good());
        ASSERT_TRUE(fileIbf.good());
        ASSERT_TRUE(fileQbf.good());

        // Test many loop
        for (std::size_t i = 0; i < 10; ++i) {
            // If no input data
            EXPECT_FALSE(task.isReady(N));

            // Send half data
            loadChannelFromOracleFile<double>(in, fileIn, N / 2);
            EXPECT_FALSE(task.isReady(N));

            // Send all data
            loadChannelFromOracleFile<double>(in, fileIn, N / 2);
            EXPECT_TRUE(task.isReady(N));
            EXPECT_FALSE(task.hasFinished(N));

            // Computing
            task.compute(N);
            EXPECT_FALSE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Compare the result
            auto resultIbf = extractVectorFromOracleFile<double>(fileIbf, N);
            auto resultQbf = extractVectorFromOracleFile<double>(fileQbf, N);
            compareChannelWithVector(resultIbf, ibf, 1e-11);
            compareChannelWithVector(resultQbf, qbf, 1e-11);
        }
    }

    TEST(DemodulationTest, testComputePrettyFsFcWithOffset) {
        static constexpr unsigned N = 2048;

        // Alloc the task
        Demodulation task(10e6, 250e6, M_PI / 7.0);
        Channel in;
        Channel &ibf = task.getOutput(0);
        Channel &qbf = task.getOutput(1);

        // Connect the input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());
        EXPECT_EQ(&task, ibf.getIn());
        EXPECT_EQ(&task, qbf.getIn());

        // Open the oracle file
        std::ifstream fileIn(std::string(ORACLE_DATA_DIR) + "/oracle_demodulation_pretty_input.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileIbf(std::string(ORACLE_DATA_DIR) + "/oracle_demodulation_pretty_wi_offset_ibf.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileQbf(std::string(ORACLE_DATA_DIR) + "/oracle_demodulation_pretty_wi_offset_qbf.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(fileIn.good());
        ASSERT_TRUE(fileIbf.good());
        ASSERT_TRUE(fileQbf.good());

        // Test many loop
        for (std::size_t i = 0; i < 10; ++i) {
            // If no input data
            EXPECT_FALSE(task.isReady(N));

            // Send half data
            loadChannelFromOracleFile<double>(in, fileIn, N / 2);
            EXPECT_FALSE(task.isReady(N));

            // Send all data
            loadChannelFromOracleFile<double>(in, fileIn, N / 2);
            EXPECT_TRUE(task.isReady(N));
            EXPECT_FALSE(task.hasFinished(N));

            // Computing
            task.compute(N);
            EXPECT_FALSE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Compare the result
            auto resultIbf = extractVectorFromOracleFile<double>(fileIbf, N);
            auto resultQbf = extractVectorFromOracleFile<double>(fileQbf, N);
            compareChannelWithVector(resultIbf, ibf, 1e-11);
            compareChannelWithVector(resultQbf, qbf, 1e-11);
        }
    }

    TEST(DemodulationTest, testComputeUglyFsFcWithoutOffset) {
        static constexpr unsigned N = 2048;

        // Alloc the task
        Demodulation task(10e6 + 5e3, 125e6, 0.0);
        Channel in;
        Channel &ibf = task.getOutput(0);
        Channel &qbf = task.getOutput(1);

        // Connect the input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());
        EXPECT_EQ(&task, ibf.getIn());
        EXPECT_EQ(&task, qbf.getIn());

        // Open the oracle file
        std::ifstream fileIn(std::string(ORACLE_DATA_DIR) + "/oracle_demodulation_ugly_input.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileIbf(std::string(ORACLE_DATA_DIR) + "/oracle_demodulation_ugly_wo_offset_ibf.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileQbf(std::string(ORACLE_DATA_DIR) + "/oracle_demodulation_ugly_wo_offset_qbf.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(fileIn.good());
        ASSERT_TRUE(fileIbf.good());
        ASSERT_TRUE(fileQbf.good());

        // Test many loop
        for (std::size_t i = 0; i < 10; ++i) {
            // If no input data
            EXPECT_FALSE(task.isReady(N));

            // Send half data
            loadChannelFromOracleFile<double>(in, fileIn, N / 2);
            EXPECT_FALSE(task.isReady(N));

            // Send all data
            loadChannelFromOracleFile<double>(in, fileIn, N / 2);
            EXPECT_TRUE(task.isReady(N));
            EXPECT_FALSE(task.hasFinished(N));

            // Computing
            task.compute(N);
            EXPECT_FALSE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Compare the result
            auto resultIbf = extractVectorFromOracleFile<double>(fileIbf, N);
            auto resultQbf = extractVectorFromOracleFile<double>(fileQbf, N);
            compareChannelWithVector(resultIbf, ibf, 1e-11);
            compareChannelWithVector(resultQbf, qbf, 1e-11);
        }
    }

    TEST(DemodulationTest, testComputeUglyFsFcWithOffset) {
        static constexpr unsigned N = 2048;

        // Alloc the task
        Demodulation task(10e6 + 5e3, 125e6, M_PI / 9.0);
        Channel in;
        Channel &ibf = task.getOutput(0);
        Channel &qbf = task.getOutput(1);

        // Connect the input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());
        EXPECT_EQ(&task, ibf.getIn());
        EXPECT_EQ(&task, qbf.getIn());

        // Open the oracle file
        std::ifstream fileIn(std::string(ORACLE_DATA_DIR) + "/oracle_demodulation_ugly_input.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileIbf(std::string(ORACLE_DATA_DIR) + "/oracle_demodulation_ugly_wi_offset_ibf.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileQbf(std::string(ORACLE_DATA_DIR) + "/oracle_demodulation_ugly_wi_offset_qbf.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(fileIn.good());
        ASSERT_TRUE(fileIbf.good());
        ASSERT_TRUE(fileQbf.good());

        // Test many loop
        for (std::size_t i = 0; i < 10; ++i) {
            // If no input data
            EXPECT_FALSE(task.isReady(N));

            // Send half data
            loadChannelFromOracleFile<double>(in, fileIn, N / 2);
            EXPECT_FALSE(task.isReady(N));

            // Send all data
            loadChannelFromOracleFile<double>(in, fileIn, N / 2);
            EXPECT_TRUE(task.isReady(N));
            EXPECT_FALSE(task.hasFinished(N));

            // Computing
            task.compute(N);
            EXPECT_FALSE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Compare the result
            auto resultIbf = extractVectorFromOracleFile<double>(fileIbf, N);
            auto resultQbf = extractVectorFromOracleFile<double>(fileQbf, N);
            compareChannelWithVector(resultIbf, ibf, 1e-11);
            compareChannelWithVector(resultQbf, qbf, 1e-11);
        }
    }

    TEST(DemodulationTest, testComputeVeryUglyFsFcWithoutOffset) {
        static constexpr unsigned N = 2048;

        // Alloc the task
        Demodulation task(63.57e4 + 12.9e2, 69.35e6, 0.0);
        Channel in;
        Channel &ibf = task.getOutput(0);
        Channel &qbf = task.getOutput(1);

        // Connect the input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());
        EXPECT_EQ(&task, ibf.getIn());
        EXPECT_EQ(&task, qbf.getIn());

        // Open the oracle file
        std::ifstream fileIn(std::string(ORACLE_DATA_DIR) + "/oracle_demodulation_very_ugly_input.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileIbf(std::string(ORACLE_DATA_DIR) + "/oracle_demodulation_very_ugly_wo_offset_ibf.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileQbf(std::string(ORACLE_DATA_DIR) + "/oracle_demodulation_very_ugly_wo_offset_qbf.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(fileIn.good());
        ASSERT_TRUE(fileIbf.good());
        ASSERT_TRUE(fileQbf.good());

        // Test many loop
        for (std::size_t i = 0; i < 10; ++i) {
            // If no input data
            EXPECT_FALSE(task.isReady(N));

            // Send half data
            loadChannelFromOracleFile<double>(in, fileIn, N / 2);
            EXPECT_FALSE(task.isReady(N));

            // Send all data
            loadChannelFromOracleFile<double>(in, fileIn, N / 2);
            EXPECT_TRUE(task.isReady(N));
            EXPECT_FALSE(task.hasFinished(N));

            // Computing
            task.compute(N);
            EXPECT_FALSE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Compare the result
            auto resultIbf = extractVectorFromOracleFile<double>(fileIbf, N);
            auto resultQbf = extractVectorFromOracleFile<double>(fileQbf, N);
            compareChannelWithVector(resultIbf, ibf, 1e-11);
            compareChannelWithVector(resultQbf, qbf, 1e-11);
        }
    }

    TEST(DemodulationTest, testComputeVeryUglyFsFcWithOffset) {
        static constexpr unsigned N = 2048;

        // Alloc the task
        Demodulation task(63.57e4 + 12.9e2, 69.35e6, M_PI / 12.0);
        Channel in;
        Channel &ibf = task.getOutput(0);
        Channel &qbf = task.getOutput(1);

        // Connect the input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());
        EXPECT_EQ(&task, ibf.getIn());
        EXPECT_EQ(&task, qbf.getIn());

        // Open the oracle file
        std::ifstream fileIn(std::string(ORACLE_DATA_DIR) + "/oracle_demodulation_very_ugly_input.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileIbf(std::string(ORACLE_DATA_DIR) + "/oracle_demodulation_very_ugly_wi_offset_ibf.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileQbf(std::string(ORACLE_DATA_DIR) + "/oracle_demodulation_very_ugly_wi_offset_qbf.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(fileIn.good());
        ASSERT_TRUE(fileIbf.good());
        ASSERT_TRUE(fileQbf.good());

        // Test many loop
        for (std::size_t i = 0; i < 10; ++i) {
            // If no input data
            EXPECT_FALSE(task.isReady(N));

            // Send half data
            loadChannelFromOracleFile<double>(in, fileIn, N / 2);
            EXPECT_FALSE(task.isReady(N));

            // Send all data
            loadChannelFromOracleFile<double>(in, fileIn, N / 2);
            EXPECT_TRUE(task.isReady(N));
            EXPECT_FALSE(task.hasFinished(N));

            // Computing
            task.compute(N);
            EXPECT_FALSE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Compare the result
            auto resultIbf = extractVectorFromOracleFile<double>(fileIbf, N);
            auto resultQbf = extractVectorFromOracleFile<double>(fileQbf, N);
            compareChannelWithVector(resultIbf, ibf, 1e-11);
            compareChannelWithVector(resultQbf, qbf, 1e-11);
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
