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
#include <dsps/Gain.h>

#include "local/Utils.h"

namespace {
    TEST(GainTest, testComputeGainPositiveReal) {
        static constexpr unsigned N = 2048;

        // Alloc the task
        Gain<double> task(2.1337);
        Channel in;
        Channel &out = task.getOutput(0);

        // Connect input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());
        EXPECT_EQ(&task, out.getIn());

        // Open the oracle file
        std::ifstream fileIn(std::string(ORACLE_DATA_DIR) + "/oracle_gain_input_real.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileOut(std::string(ORACLE_DATA_DIR) + "/oracle_gain_output_positive_real.bin", std::ios_base::in|std::ios_base::binary);
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

            // Computing
            task.compute(N);
            EXPECT_FALSE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Extract the result
            std::vector<double> result = extractVectorFromOracleFile<double>(fileOut, N);
            compareChannelWithVector(result, out, 0.0);
        }
    }

    TEST(GainTest, testComputeGainNegativeReal) {
        static constexpr unsigned N = 2048;

        // Alloc the task
        Gain<double> task(0.421337);
        Channel in;
        Channel &out = task.getOutput(0);

        // Connect input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());
        EXPECT_EQ(&task, out.getIn());

        // Open the oracle file
        std::ifstream fileIn(std::string(ORACLE_DATA_DIR) + "/oracle_gain_input_real.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileOut(std::string(ORACLE_DATA_DIR) + "/oracle_gain_output_negative_real.bin", std::ios_base::in|std::ios_base::binary);
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

            // Computing
            task.compute(N);
            EXPECT_FALSE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Extract the result
            std::vector<double> result = extractVectorFromOracleFile<double>(fileOut, N);
            compareChannelWithVector(result, out, 0.0);
        }
    }

    TEST(GainTest, testComputeGainPositiveComplex) {
        static constexpr unsigned N = 2048;

        // Alloc the task
        Gain< std::complex<double> > task(2.1337);
        Channel in;
        Channel &out = task.getOutput(0);

        // Connect input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());
        EXPECT_EQ(&task, out.getIn());

        // Open the oracle file
        std::ifstream fileIn(std::string(ORACLE_DATA_DIR) + "/oracle_gain_input_complex.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileOut(std::string(ORACLE_DATA_DIR) + "/oracle_gain_output_positive_complex.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(fileIn.good());
        ASSERT_TRUE(fileOut.good());

        for (std::size_t i = 0; i < 10; ++i) {
            // If no input data
            EXPECT_FALSE(task.isReady(N));

            // Send half data
            loadChannelFromOracleFile< std::complex<double> >(in, fileIn, N/2);
            EXPECT_FALSE(task.isReady(N));

            // Send all data
            loadChannelFromOracleFile< std::complex<double> >(in, fileIn, N/2);
            EXPECT_TRUE(task.isReady(N));

            // Computing
            task.compute(N);
            EXPECT_FALSE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Extract the result
            std::vector< std::complex<double> > result = extractVectorFromOracleFile< std::complex<double> >(fileOut, N);
            compareChannelWithVector(result, out, 0.0);
        }
    }

    TEST(GainTest, testComputeGainNegativeComplex) {
        static constexpr unsigned N = 2048;

        // Alloc the task
        Gain< std::complex<double> > task(0.421337);
        Channel in;
        Channel &out = task.getOutput(0);

        // Connect input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());
        EXPECT_EQ(&task, out.getIn());

        // Open the oracle file
        std::ifstream fileIn(std::string(ORACLE_DATA_DIR) + "/oracle_gain_input_complex.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileOut(std::string(ORACLE_DATA_DIR) + "/oracle_gain_output_negative_complex.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(fileIn.good());
        ASSERT_TRUE(fileOut.good());

        for (std::size_t i = 0; i < 10; ++i) {
            // If no input data
            EXPECT_FALSE(task.isReady(N));

            // Send half data
            loadChannelFromOracleFile< std::complex<double> >(in, fileIn, N/2);
            EXPECT_FALSE(task.isReady(N));

            // Send all data
            loadChannelFromOracleFile< std::complex<double> >(in, fileIn, N/2);
            EXPECT_TRUE(task.isReady(N));

            // Computing
            task.compute(N);
            EXPECT_FALSE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Extract the result
            std::vector< std::complex<double> > result = extractVectorFromOracleFile< std::complex<double> >(fileOut, N);
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
