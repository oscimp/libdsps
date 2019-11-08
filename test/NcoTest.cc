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
#include <dsps/Nco.h>

#include "local/Utils.h"

namespace {
    TEST(NcoTest, testComputePrettyFsFc) {
        static constexpr unsigned N = 2048;
        static constexpr double AMPLITUDE = 2.5;
        static constexpr double FC = 10e6;
        static constexpr double FS = 250e6;

        // Alloc the task
        Nco task(AMPLITUDE, FC, FS);
        Channel &outCos = task.getOutput(0);
        Channel &outSin = task.getOutput(1);

        // Check if the channel was correct
        EXPECT_EQ(&task, outCos.getIn());
        EXPECT_EQ(&task, outSin.getIn());

        // Open the oracle file
        std::ifstream fileOutCos(std::string(ORACLE_DATA_DIR) + "/oracle_nco_pretty_output_cos.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileOutSin(std::string(ORACLE_DATA_DIR) + "/oracle_nco_pretty_output_sin.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(fileOutCos.good());
        ASSERT_TRUE(fileOutSin.good());

        for (std::size_t i = 0; i < 1000; ++i) {
            // Always ready
            EXPECT_TRUE(task.isReady(N));
            EXPECT_FALSE(task.hasFinished(N));

            // Computing
            task.compute(N);
            EXPECT_TRUE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Compare the result
            std::vector<double> result = extractVectorFromOracleFile<double>(fileOutCos, N);
            compareChannelWithVector(result, outCos, 1e-9);
            result = extractVectorFromOracleFile<double>(fileOutSin, N);
            compareChannelWithVector(result, outSin, 1e-9);
        }
    }

    TEST(NcoTest, testComputeUglyFsFc) {
        static constexpr unsigned N = 2048;
        static constexpr double AMPLITUDE = 1.25;
        static constexpr double FC = 10e6 + 5e3;
        static constexpr double FS = 125e6;

        // Alloc the task
        Nco task(AMPLITUDE, FC, FS);
        Channel &outCos = task.getOutput(0);
        Channel &outSin = task.getOutput(1);

        // Check if the channel was correct
        EXPECT_EQ(&task, outCos.getIn());
        EXPECT_EQ(&task, outSin.getIn());

        // Open the oracle file
        std::ifstream fileOutCos(std::string(ORACLE_DATA_DIR) + "/oracle_nco_ugly_output_cos.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileOutSin(std::string(ORACLE_DATA_DIR) + "/oracle_nco_ugly_output_sin.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(fileOutCos.good());
        ASSERT_TRUE(fileOutSin.good());

        for (std::size_t i = 0; i < 1000; ++i) {
            // Always ready
            EXPECT_TRUE(task.isReady(N));
            EXPECT_FALSE(task.hasFinished(N));

            // Computing
            task.compute(N);
            EXPECT_TRUE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Compare the result
            std::vector<double> result = extractVectorFromOracleFile<double>(fileOutCos, N);
            compareChannelWithVector(result, outCos, 1e-9);
            result = extractVectorFromOracleFile<double>(fileOutSin, N);
            compareChannelWithVector(result, outSin, 1e-9);
        }
    }

    TEST(NcoTest, testComputeVeryUglyFsFc) {
        static constexpr unsigned N = 2048;
        static constexpr double AMPLITUDE = 0.525;
        static constexpr double FC = 87.3974e4 + 3.5e2;
        static constexpr double FS = 578.38e6;

        // Alloc the task
        Nco task(AMPLITUDE, FC, FS);
        Channel &outCos = task.getOutput(0);
        Channel &outSin = task.getOutput(1);

        // Check if the channel was correct
        EXPECT_EQ(&task, outCos.getIn());
        EXPECT_EQ(&task, outSin.getIn());

        // Open the oracle file
        std::ifstream fileOutCos(std::string(ORACLE_DATA_DIR) + "/oracle_nco_very_ugly_output_cos.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileOutSin(std::string(ORACLE_DATA_DIR) + "/oracle_nco_very_ugly_output_sin.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(fileOutCos.good());
        ASSERT_TRUE(fileOutSin.good());

        for (std::size_t i = 0; i < 1000; ++i) {
            // Always ready
            EXPECT_TRUE(task.isReady(N));
            EXPECT_FALSE(task.hasFinished(N));

            // Computing
            task.compute(N);
            EXPECT_TRUE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Compare the result
            std::vector<double> result = extractVectorFromOracleFile<double>(fileOutCos, N);
            compareChannelWithVector(result, outCos, 1e-9);
            result = extractVectorFromOracleFile<double>(fileOutSin, N);
            compareChannelWithVector(result, outSin, 1e-9);
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
