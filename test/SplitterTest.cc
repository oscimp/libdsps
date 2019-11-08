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
#include <dsps/Splitter.h>

#include "local/Utils.h"

namespace {
    TEST(SplitterTest, testCompute2CHReal) {
        static constexpr unsigned N = 2048;

        // Alloc the task
        Splitter<double> task(2);
        Channel in;
        Channel &out1 = task.getOutput(0);
        Channel &out2 = task.getOutput(1);

        // Connect input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());
        EXPECT_EQ(&task, out1.getIn());
        EXPECT_EQ(&task, out2.getIn());

        // Open the oracle file
        std::ifstream fileIn(std::string(ORACLE_DATA_DIR) + "/oracle_splitter_input_real.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileOut(std::string(ORACLE_DATA_DIR) + "/oracle_splitter_input_real.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(fileIn.good());

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
            auto result = extractVectorFromOracleFile<double>(fileOut, N);
            compareChannelWithVector(result, out1, 0);
            compareChannelWithVector(result, out2, 0);
        }
    }

    TEST(SplitterTest, testCompute10CHReal) {
        static constexpr unsigned N = 2048;

        // Alloc the task
        Splitter<double> task(10);
        Channel in;
        Channel &out1 = task.getOutput(0);
        Channel &out2 = task.getOutput(1);
        Channel &out3 = task.getOutput(2);
        Channel &out4 = task.getOutput(3);
        Channel &out5 = task.getOutput(4);
        Channel &out6 = task.getOutput(5);
        Channel &out7 = task.getOutput(6);
        Channel &out8 = task.getOutput(7);
        Channel &out9 = task.getOutput(8);
        Channel &out10 = task.getOutput(9);

        // Connect input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());
        EXPECT_EQ(&task, out1.getIn());
        EXPECT_EQ(&task, out2.getIn());
        EXPECT_EQ(&task, out3.getIn());
        EXPECT_EQ(&task, out4.getIn());
        EXPECT_EQ(&task, out5.getIn());
        EXPECT_EQ(&task, out6.getIn());
        EXPECT_EQ(&task, out7.getIn());
        EXPECT_EQ(&task, out8.getIn());
        EXPECT_EQ(&task, out9.getIn());
        EXPECT_EQ(&task, out10.getIn());

        // Open the oracle file
        std::ifstream fileIn(std::string(ORACLE_DATA_DIR) + "/oracle_splitter_input_real.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileOut(std::string(ORACLE_DATA_DIR) + "/oracle_splitter_input_real.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(fileIn.good());

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
            auto result = extractVectorFromOracleFile<double>(fileOut, N);
            compareChannelWithVector(result, out1, 0);
            compareChannelWithVector(result, out2, 0);
            compareChannelWithVector(result, out3, 0);
            compareChannelWithVector(result, out4, 0);
            compareChannelWithVector(result, out5, 0);
            compareChannelWithVector(result, out6, 0);
            compareChannelWithVector(result, out7, 0);
            compareChannelWithVector(result, out8, 0);
            compareChannelWithVector(result, out9, 0);
            compareChannelWithVector(result, out10, 0);
        }
    }

    TEST(SplitterTest, testCompute2CHComplex) {
        static constexpr unsigned N = 2048;

        // Alloc the task
        Splitter< std::complex<double> > task(2);
        Channel in;
        Channel &out1 = task.getOutput(0);
        Channel &out2 = task.getOutput(1);

        // Connect input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());
        EXPECT_EQ(&task, out1.getIn());
        EXPECT_EQ(&task, out2.getIn());

        // Open the oracle file
        std::ifstream fileIn(std::string(ORACLE_DATA_DIR) + "/oracle_splitter_input_complex.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileOut(std::string(ORACLE_DATA_DIR) + "/oracle_splitter_input_complex.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(fileIn.good());

        // Test many loop
        for (std::size_t i = 0; i < 10; ++i) {
            // If no input data
            EXPECT_FALSE(task.isReady(N));

            // Send half data
            loadChannelFromOracleFile< std::complex<double> >(in, fileIn, N / 2);
            EXPECT_FALSE(task.isReady(N));

            // Send all data
            loadChannelFromOracleFile< std::complex<double> >(in, fileIn, N / 2);
            EXPECT_TRUE(task.isReady(N));
            EXPECT_FALSE(task.hasFinished(N));

            // Computing
            task.compute(N);
            EXPECT_FALSE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Compare the result
            auto result = extractVectorFromOracleFile< std::complex<double> >(fileOut, N);
            compareChannelWithVector(result, out1, 0);
            compareChannelWithVector(result, out2, 0);
        }
    }

    TEST(SplitterTest, testCompute10CHComplex) {
        static constexpr unsigned N = 2048;

        // Alloc the task
        Splitter< std::complex<double> > task(10);
        Channel in;
        Channel &out1 = task.getOutput(0);
        Channel &out2 = task.getOutput(1);
        Channel &out3 = task.getOutput(2);
        Channel &out4 = task.getOutput(3);
        Channel &out5 = task.getOutput(4);
        Channel &out6 = task.getOutput(5);
        Channel &out7 = task.getOutput(6);
        Channel &out8 = task.getOutput(7);
        Channel &out9 = task.getOutput(8);
        Channel &out10 = task.getOutput(9);

        // Connect input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());
        EXPECT_EQ(&task, out1.getIn());
        EXPECT_EQ(&task, out2.getIn());
        EXPECT_EQ(&task, out3.getIn());
        EXPECT_EQ(&task, out4.getIn());
        EXPECT_EQ(&task, out5.getIn());
        EXPECT_EQ(&task, out6.getIn());
        EXPECT_EQ(&task, out7.getIn());
        EXPECT_EQ(&task, out8.getIn());
        EXPECT_EQ(&task, out9.getIn());
        EXPECT_EQ(&task, out10.getIn());

        // Open the oracle file
        std::ifstream fileIn(std::string(ORACLE_DATA_DIR) + "/oracle_splitter_input_complex.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileOut(std::string(ORACLE_DATA_DIR) + "/oracle_splitter_input_complex.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(fileIn.good());

        // Test many loop
        for (std::size_t i = 0; i < 10; ++i) {
            // If no input data
            EXPECT_FALSE(task.isReady(N));

            // Send half data
            loadChannelFromOracleFile< std::complex<double> >(in, fileIn, N / 2);
            EXPECT_FALSE(task.isReady(N));

            // Send all data
            loadChannelFromOracleFile< std::complex<double> >(in, fileIn, N / 2);
            EXPECT_TRUE(task.isReady(N));
            EXPECT_FALSE(task.hasFinished(N));

            // Computing
            task.compute(N);
            EXPECT_FALSE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Compare the result
            auto result = extractVectorFromOracleFile< std::complex<double> >(fileOut, N);
            compareChannelWithVector(result, out1, 0);
            compareChannelWithVector(result, out2, 0);
            compareChannelWithVector(result, out3, 0);
            compareChannelWithVector(result, out4, 0);
            compareChannelWithVector(result, out5, 0);
            compareChannelWithVector(result, out6, 0);
            compareChannelWithVector(result, out7, 0);
            compareChannelWithVector(result, out8, 0);
            compareChannelWithVector(result, out9, 0);
            compareChannelWithVector(result, out10, 0);
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
