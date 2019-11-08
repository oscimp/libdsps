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
#include <dsps/Abs.h>

#include "local/Utils.h"

namespace {
    TEST(AbsTest, testComputeDouble) {
        static constexpr unsigned N = 2048;

        // Alloc the task
        Channel in;
        Abs<double, double> task;

        // Connect the input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());

        // Open the oracle file
        std::ifstream fileIn(std::string(ORACLE_DATA_DIR) + "/oracle_abs_double_input.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileOut(std::string(ORACLE_DATA_DIR) + "/oracle_abs_double_output.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(fileIn.good());
        ASSERT_TRUE(fileOut.good());

        // If no input data
        EXPECT_FALSE(task.isReady(N));
        for (std::size_t i = 0; i < 10; ++i) {
            // Half data
            loadChannelFromOracleFile<double>(in, fileIn, N/2);
            EXPECT_FALSE(task.isReady(N));

            // All data
            loadChannelFromOracleFile<double>(in, fileIn, N/2);
            EXPECT_TRUE(task.isReady(N));

            // Computing
            task.compute(N);
            EXPECT_FALSE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Compare the result
            auto result = extractVectorFromOracleFile<double>(fileOut, N);
            compareChannelWithVector(result, task.getOutput(0), 0.0);
        }
    }

    TEST(AbsTest, testComputeComplex) {
        static constexpr unsigned N = 2048;

        // Alloc the task
        Channel in;
        Abs<std::complex<double>, double> task;

        // Connect the input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());

        // Open the oracle file
        std::ifstream fileIn(std::string(ORACLE_DATA_DIR) + "/oracle_abs_complex_input.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileOut(std::string(ORACLE_DATA_DIR) + "/oracle_abs_complex_output.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(fileIn.good());
        ASSERT_TRUE(fileOut.good());

        // If no input data
        EXPECT_FALSE(task.isReady(N));
        for (std::size_t i = 0; i < 10; ++i) {
            // Half data
            loadChannelFromOracleFile< std::complex<double> >(in, fileIn, N/2);
            EXPECT_FALSE(task.isReady(N));

            // All data
            loadChannelFromOracleFile< std::complex<double> >(in, fileIn, N/2);
            EXPECT_TRUE(task.isReady(N));

            // Computing
            task.compute(N);
            EXPECT_FALSE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Compare the result
            auto result = extractVectorFromOracleFile<double>(fileOut, N);
            compareChannelWithVector(result, task.getOutput(0), 0.0);
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
