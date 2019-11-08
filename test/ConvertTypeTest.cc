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
#include <dsps/ConvertType.h>

#include "local/Utils.h"

namespace {
    TEST(ConvertTypeTest, testComputeDouble2Int8) {
        static constexpr unsigned N = 2048;

        // Alloc the task
        ConvertType<double, std::int64_t> task(8, 2.5);
        Channel in;
        Channel &out = task.getOutput(0);

        // Connect input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());
        EXPECT_EQ(&task, out.getIn());

        // Open the oracle file
        std::ifstream fileIn(std::string(ORACLE_DATA_DIR) + "/oracle_convert_type_real.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileOut(std::string(ORACLE_DATA_DIR) + "/oracle_convert_type_int08.bin", std::ios_base::in|std::ios_base::binary);
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
            std::vector<std::int64_t> result = extractVectorFromOracleFile<std::int64_t>(fileOut, N);

            // Compare the result
            std::vector<std::int64_t> values;
            EXPECT_EQ(N, out.size(sizeof(std::int64_t)));
            out.receive(values, N);

            std::int64_t max = 0;
            for (std::size_t j = 0; j < N; ++j) {
                if (max < std::abs(values[j])) {
                    max = std::abs(values[j]);
                }

                ASSERT_EQ(result[j], values[j]);
            }

            // Test the max value
            ASSERT_EQ(max, std::pow(2, 7) - 1);
        }
    }

    TEST(ConvertTypeTest, testComputeDouble2Int16) {
        static constexpr unsigned N = 2048;

        // Alloc the task
        ConvertType<double, std::int64_t> task(16, 2.5);
        Channel in;
        Channel &out = task.getOutput(0);

        // Connect input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());
        EXPECT_EQ(&task, out.getIn());

        // Open the oracle file
        std::ifstream fileIn(std::string(ORACLE_DATA_DIR) + "/oracle_convert_type_real.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileOut(std::string(ORACLE_DATA_DIR) + "/oracle_convert_type_int16.bin", std::ios_base::in|std::ios_base::binary);
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
            std::vector<std::int64_t> result = extractVectorFromOracleFile<std::int64_t>(fileOut, N);
            std::int64_t max = 0;
            std::size_t limit = out.size(sizeof(std::int64_t));
            std::vector<std::int64_t> values;
            out.receive(values, N);
            for (std::size_t j = 0; j < limit; ++j) {
                std::int64_t val = values[j];
                if (max < std::abs(val)) {
                    max = std::abs(val);
                }
                ASSERT_EQ(result[j], val);
            }

            // Test the max value
            ASSERT_EQ(max, std::pow(2, 15) - 1);
        }
    }

    TEST(ConvertTypeTest, testComputeDouble2Int24) {
        static constexpr unsigned N = 2048;

        // Alloc the task
        ConvertType<double, std::int64_t> task(24, 2.5);
        Channel in;
        Channel &out = task.getOutput(0);

        // Connect input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());
        EXPECT_EQ(&task, out.getIn());

        // Open the oracle file
        std::ifstream fileIn(std::string(ORACLE_DATA_DIR) + "/oracle_convert_type_real.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileOut(std::string(ORACLE_DATA_DIR) + "/oracle_convert_type_int24.bin", std::ios_base::in|std::ios_base::binary);
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
            std::vector<std::int64_t> result = extractVectorFromOracleFile<std::int64_t>(fileOut, N);
            std::int64_t max = 0;
            std::size_t limit = out.size(sizeof(std::int64_t));
            std::vector<std::int64_t> values;
            out.receive(values, N);
            for (std::size_t j = 0; j < limit; ++j) {
                std::int64_t val = values[j];
                if (max < std::abs(val)) {
                    max = std::abs(val);
                }
                ASSERT_EQ(result[j], val);
            }

            // Test the max value
            ASSERT_EQ(max, std::pow(2, 23) - 1);
        }
    }

    TEST(ConvertTypeTest, testComputeDouble2Int32) {
        static constexpr unsigned N = 2048;

        // Alloc the task
        ConvertType<double, std::int64_t> task(32, 2.5);
        Channel in;
        Channel &out = task.getOutput(0);

        // Connect input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());
        EXPECT_EQ(&task, out.getIn());

        // Open the oracle file
        std::ifstream fileIn(std::string(ORACLE_DATA_DIR) + "/oracle_convert_type_real.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileOut(std::string(ORACLE_DATA_DIR) + "/oracle_convert_type_int32.bin", std::ios_base::in|std::ios_base::binary);
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
            std::vector<std::int64_t> result = extractVectorFromOracleFile<std::int64_t>(fileOut, N);
            std::int64_t max = 0;
            std::size_t limit = out.size(sizeof(std::int64_t));
            std::vector<std::int64_t> values;
            out.receive(values, N);
            for (std::size_t j = 0; j < limit; ++j) {
                std::int64_t val = values[j];
                if (max < std::abs(val)) {
                    max = std::abs(val);
                }
                ASSERT_EQ(result[j], val);
            }

            // Test the max value
            ASSERT_EQ(max, std::pow(2, 31) - 1);
        }
    }

    TEST(ConvertTypeTest, testComputeInt82Double) {
        static constexpr unsigned N = 2048;

        // Alloc the task
        ConvertType<std::int64_t, double> task;
        Channel in;
        Channel &out = task.getOutput(0);

        // Connect input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());
        EXPECT_EQ(&task, out.getIn());

        // Open the oracle file
        std::ifstream fileIn(std::string(ORACLE_DATA_DIR) + "/oracle_convert_type_int08.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileOut(std::string(ORACLE_DATA_DIR) + "/oracle_convert_type_real08.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(fileIn.good());
        ASSERT_TRUE(fileOut.good());

        for (std::size_t i = 0; i < 10; ++i) {
            // If no input data
            EXPECT_FALSE(task.isReady(N));

            // Send half data
            loadChannelFromOracleFile<std::int64_t>(in, fileIn, N/2);
            EXPECT_FALSE(task.isReady(N));

            // Send all data
            loadChannelFromOracleFile<std::int64_t>(in, fileIn, N/2);
            EXPECT_TRUE(task.isReady(N));

            // Computing
            task.compute(N);
            EXPECT_FALSE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Extract the result
            auto result = extractVectorFromOracleFile<double>(fileOut, N);
            std::int64_t max = 0;
            std::size_t limit = out.size(sizeof(double));
            std::vector<double> values;
            out.receive(values, N);
            for (std::size_t j = 0; j < limit; ++j) {
                double val = values[j];
                if (max < std::abs(val)) {
                    max = std::abs(val);
                }
                ASSERT_EQ(result[j], val);
            }

            // Test the max value
            ASSERT_EQ(max, std::pow(2, 7) - 1);
        }
    }

    TEST(ConvertTypeTest, testComputeInt162Double) {
        static constexpr unsigned N = 2048;

        // Alloc the task
        ConvertType<std::int64_t, double> task;
        Channel in;
        Channel &out = task.getOutput(0);

        // Connect input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());
        EXPECT_EQ(&task, out.getIn());

        // Open the oracle file
        std::ifstream fileIn(std::string(ORACLE_DATA_DIR) + "/oracle_convert_type_int16.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileOut(std::string(ORACLE_DATA_DIR) + "/oracle_convert_type_real16.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(fileIn.good());
        ASSERT_TRUE(fileOut.good());

        for (std::size_t i = 0; i < 10; ++i) {
            // If no input data
            EXPECT_FALSE(task.isReady(N));

            // Send half data
            loadChannelFromOracleFile<std::int64_t>(in, fileIn, N/2);
            EXPECT_FALSE(task.isReady(N));

            // Send all data
            loadChannelFromOracleFile<std::int64_t>(in, fileIn, N/2);
            EXPECT_TRUE(task.isReady(N));

            // Computing
            task.compute(N);
            EXPECT_FALSE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Extract the result
            auto result = extractVectorFromOracleFile<double>(fileOut, N);
            std::int64_t max = 0;
            std::size_t limit = out.size(sizeof(double));
            std::vector<double> values;
            out.receive(values, N);
            for (std::size_t j = 0; j < limit; ++j) {
                double val = values[j];
                if (max < std::abs(val)) {
                    max = std::abs(val);
                }
                ASSERT_EQ(result[j], val);
            }

            // Test the max value
            ASSERT_EQ(max, std::pow(2, 15) - 1);
        }
    }

    TEST(ConvertTypeTest, testComputeInt242Double) {
        static constexpr unsigned N = 2048;

        // Alloc the task
        ConvertType<std::int64_t, double> task;
        Channel in;
        Channel &out = task.getOutput(0);

        // Connect input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());
        EXPECT_EQ(&task, out.getIn());

        // Open the oracle file
        std::ifstream fileIn(std::string(ORACLE_DATA_DIR) + "/oracle_convert_type_int24.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileOut(std::string(ORACLE_DATA_DIR) + "/oracle_convert_type_real24.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(fileIn.good());
        ASSERT_TRUE(fileOut.good());

        for (std::size_t i = 0; i < 10; ++i) {
            // If no input data
            EXPECT_FALSE(task.isReady(N));

            // Send half data
            loadChannelFromOracleFile<std::int64_t>(in, fileIn, N/2);
            EXPECT_FALSE(task.isReady(N));

            // Send all data
            loadChannelFromOracleFile<std::int64_t>(in, fileIn, N/2);
            EXPECT_TRUE(task.isReady(N));

            // Computing
            task.compute(N);
            EXPECT_FALSE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Extract the result
            auto result = extractVectorFromOracleFile<double>(fileOut, N);
            std::int64_t max = 0;
            std::size_t limit = out.size(sizeof(double));
            std::vector<double> values;
            out.receive(values, N);
            for (std::size_t j = 0; j < limit; ++j) {
                double val = values[j];
                if (max < std::abs(val)) {
                    max = std::abs(val);
                }
                ASSERT_EQ(result[j], val);
            }

            // Test the max value
            ASSERT_EQ(max, std::pow(2, 23) - 1);
        }
    }

    TEST(ConvertTypeTest, testComputeInt322Double) {
        static constexpr unsigned N = 2048;

        // Alloc the task
        ConvertType<std::int64_t, double> task;
        Channel in;
        Channel &out = task.getOutput(0);

        // Connect input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());
        EXPECT_EQ(&task, out.getIn());

        // Open the oracle file
        std::ifstream fileIn(std::string(ORACLE_DATA_DIR) + "/oracle_convert_type_int32.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileOut(std::string(ORACLE_DATA_DIR) + "/oracle_convert_type_real32.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(fileIn.good());
        ASSERT_TRUE(fileOut.good());

        for (std::size_t i = 0; i < 10; ++i) {
            // If no input data
            EXPECT_FALSE(task.isReady(N));

            // Send half data
            loadChannelFromOracleFile<std::int64_t>(in, fileIn, N/2);
            EXPECT_FALSE(task.isReady(N));

            // Send all data
            loadChannelFromOracleFile<std::int64_t>(in, fileIn, N/2);
            EXPECT_TRUE(task.isReady(N));

            // Computing
            task.compute(N);
            EXPECT_FALSE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Extract the result
            auto result = extractVectorFromOracleFile<double>(fileOut, N);
            std::int64_t max = 0;
            std::size_t limit = out.size(sizeof(double));
            std::vector<double> values;
            out.receive(values, N);
            for (std::size_t j = 0; j < limit; ++j) {
                double val = values[j];
                if (max < std::abs(val)) {
                    max = std::abs(val);
                }
                ASSERT_EQ(result[j], val);
            }

            // Test the max value
            ASSERT_EQ(max, std::pow(2, 31) - 1);
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
