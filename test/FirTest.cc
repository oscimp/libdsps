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
#include <dsps/Fir.h>

#include "local/Utils.h"

namespace {
    TEST(FirTest, testComputeDecim10) {
        static constexpr unsigned N = 2048;
        static constexpr unsigned NFir = 128;

        // Alloc the task
        Fir<double> task(std::string(ORACLE_DATA_DIR) + "/kaiser128_40", 10);
        Channel in;
        Channel &out = task.getOutput(0);

        // Connect input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());
        EXPECT_EQ(&task, out.getIn());

        // Open the oracle file
        std::ifstream fileIn(std::string(ORACLE_DATA_DIR) + "/oracle_fir_input_decim10.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileOut(std::string(ORACLE_DATA_DIR) + "/oracle_fir_output_decim10.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(fileIn.good());
        ASSERT_TRUE(fileOut.good());

        // Load initial buffer
        loadChannelFromOracleFile<double>(in, fileIn, NFir);

        // Send half data
        for (std::size_t j = 0; j < 5; ++j) {
            for (std::size_t i = 0; i < 5; ++i) {
                loadChannelFromOracleFile<double>(in, fileIn, N);
                EXPECT_FALSE(task.isReady(N));
            }

            // Send all data
            for (std::size_t i = 0; i < 5; ++i) {
                loadChannelFromOracleFile<double>(in, fileIn, N);
                EXPECT_EQ(i == 4, task.isReady(N));
            }
            EXPECT_FALSE(task.hasFinished(N));

            // Computing
            task.compute(N);
            EXPECT_FALSE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Compare the result
            auto result = extractVectorFromOracleFile<double>(fileOut, N);
            compareChannelWithVector(result, out, 1e-14);
        }
    }

    TEST(FirTest, testComputeDecim100) {
        static constexpr unsigned N = 2048;
        static constexpr unsigned NFir = 128;

        // Alloc the task
        Fir<double> task(std::string(ORACLE_DATA_DIR) + "/kaiser128_40", 100);
        Channel in;
        Channel &out = task.getOutput(0);

        // Connect input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());
        EXPECT_EQ(&task, out.getIn());

        // Open the oracle file
        std::ifstream fileIn(std::string(ORACLE_DATA_DIR) + "/oracle_fir_input_decim100.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileOut(std::string(ORACLE_DATA_DIR) + "/oracle_fir_output_decim100.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(fileIn.good());
        ASSERT_TRUE(fileOut.good());

        // Load initial buffer
        loadChannelFromOracleFile<double>(in, fileIn, NFir);

        // Send half data
        for (std::size_t j = 0; j < 5; ++j) {
            for (std::size_t i = 0; i < 50; ++i) {
                loadChannelFromOracleFile<double>(in, fileIn, N);
                EXPECT_FALSE(task.isReady(N));
            }

            // Send all data
            for (std::size_t i = 0; i < 50; ++i) {
                loadChannelFromOracleFile<double>(in, fileIn, N);
                EXPECT_EQ(i == 49, task.isReady(N));
            }
            EXPECT_FALSE(task.hasFinished(N));

            // Computing
            task.compute(N);
            EXPECT_FALSE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Compare the result
            auto result = extractVectorFromOracleFile<double>(fileOut, N);
            compareChannelWithVector(result, out, 1e-14);
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
