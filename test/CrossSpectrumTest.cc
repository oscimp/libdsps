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
#include <dsps/CrossSpectrum.h>

#include "local/Utils.h"

namespace {

    TEST(CrossSpectrumTest, testComputeCross) {
        static constexpr unsigned N = 2048;

        // Alloc the task
        CrossSpectrum task;
        Channel ch1;
        Channel ch2;
        Channel &out = task.getOutput(0);

        // Connect input
        task.setInput(ch1, 0);
        task.setInput(ch2, 1);

        // Check if the channel was correct
        EXPECT_EQ(&task, ch1.getOut());
        EXPECT_EQ(&task, ch2.getOut());
        EXPECT_EQ(&task, out.getIn());

        // Open the oracle file
        std::ifstream fileCh1(std::string(ORACLE_DATA_DIR) + "/oracle_cross_spectrum_ch1.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileCh2(std::string(ORACLE_DATA_DIR) + "/oracle_cross_spectrum_ch2.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileOut(std::string(ORACLE_DATA_DIR) + "/oracle_cross_spectrum_output_cross.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(fileCh1.good());
        ASSERT_TRUE(fileCh2.good());
        ASSERT_TRUE(fileOut.good());

        // If no input data
        EXPECT_FALSE(task.isReady(N));

        // Send half data
        loadChannelFromOracleFile< std::complex<double> >(ch1, fileCh1, N/2);
        EXPECT_FALSE(task.isReady(N));
        loadChannelFromOracleFile< std::complex<double> >(ch2, fileCh2, N/2);
        EXPECT_FALSE(task.isReady(N));

        // Send all data
        loadChannelFromOracleFile< std::complex<double> >(ch1, fileCh1, N/2);
        EXPECT_FALSE(task.isReady(N));
        loadChannelFromOracleFile< std::complex<double> >(ch2, fileCh2, N/2);
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

    TEST(CrossSpectrumTest, testComputeAuto) {
        static constexpr unsigned N = 2048;

        // Alloc the task
        CrossSpectrum task;
        Channel ch1;
        Channel ch2;
        Channel &out = task.getOutput(0);

        // Connect input
        task.setInput(ch1, 0);
        task.setInput(ch2, 1);

        // Check if the channel was correct
        EXPECT_EQ(&task, ch1.getOut());
        EXPECT_EQ(&task, ch2.getOut());
        EXPECT_EQ(&task, out.getIn());

        // Open the oracle file
        std::ifstream fileCh1(std::string(ORACLE_DATA_DIR) + "/oracle_cross_spectrum_ch1.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileCh2(std::string(ORACLE_DATA_DIR) + "/oracle_cross_spectrum_ch1.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileOut(std::string(ORACLE_DATA_DIR) + "/oracle_cross_spectrum_output_auto.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(fileCh1.good());
        ASSERT_TRUE(fileCh2.good());
        ASSERT_TRUE(fileOut.good());

        // If no input data
        EXPECT_FALSE(task.isReady(N));

        // Send half data
        loadChannelFromOracleFile< std::complex<double> >(ch1, fileCh1, N/2);
        EXPECT_FALSE(task.isReady(N));
        loadChannelFromOracleFile< std::complex<double> >(ch2, fileCh2, N/2);
        EXPECT_FALSE(task.isReady(N));

        // Send all data
        loadChannelFromOracleFile< std::complex<double> >(ch1, fileCh1, N/2);
        EXPECT_FALSE(task.isReady(N));
        loadChannelFromOracleFile< std::complex<double> >(ch2, fileCh2, N/2);
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

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);
    ::testing::GTEST_FLAG(throw_on_failure) = true;
    ::testing::FLAGS_gtest_death_test_style = "fast";
    return RUN_ALL_TESTS();
}
