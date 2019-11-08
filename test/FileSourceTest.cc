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
#include <dsps/FileSource.h>

#include "local/Utils.h"

namespace {
    template <typename T>
    std::vector<T> loadValuesFromPlainFile(const std::string &filename) {
        std::vector<T> values;

        // Open the file
        std::ifstream file(filename);
        EXPECT_TRUE(file.good());

        // Read all values
        T value;
        while (file >> value) {
            if (file.eof()) {
                break;
            }

            values.push_back(value);
        }

        return values;
    }

    template <>
    std::vector<std::complex<double>> loadValuesFromPlainFile(const std::string &filename) {
        std::vector<std::complex<double>> values;

        // Open the file
        std::ifstream file(filename);
        EXPECT_TRUE(file.good());

        // Read all values
        double real, imag;
        while (file >> real >> imag) {
            if (file.eof()) {
                break;
            }

            values.push_back(std::complex<double>(real, imag));
        }

        return values;
    }

    template <typename T>
    std::vector<T> loadValuesFromBinaryFile(const std::string &filename) {
        std::vector<T> values;

        // Open the file
        std::ifstream file(filename, std::ios::binary);
        EXPECT_TRUE(file.good());

        // Read all values
        T value;
        for (;;) {
            file.read(reinterpret_cast<char*>(&value), sizeof(T));
            if (file.eof()) {
                break;
            }

            values.push_back(value);
        }

        return values;
    }

    void compareChannelWithVector(std::vector<std::int64_t> &expected, std::size_t offset, Channel &actual, std::size_t N) {
        // The size must be match
        ASSERT_EQ(N, actual.size(sizeof(std::int64_t)));

        // Get the data
        std::vector<std::int64_t> values(N);
        actual.receive(values, N);

        // Compare the values
        for (std::size_t i = 0; i < N; ++i) {
            EXPECT_EQ(expected[offset + i], values[i]);
        }
    }

    void compareChannelWithVector(std::vector<double> &expected, std::size_t offset, Channel &actual, std::size_t N) {
        // The size must be match
        ASSERT_EQ(N, actual.size(sizeof(double)));

        // Get the data
        std::vector<double> values(N);
        actual.receive(values, N);

        // Compare the values
        for (std::size_t i = 0; i < N; ++i) {
            expect_eq_double(expected[offset + i], values[i], 0.0);
        }
    }

    void compareChannelWithVector(std::vector< std::complex<double> > &expected, std::size_t offset, Channel &actual, std::size_t N) {
        // The size must be match
        ASSERT_EQ(N, actual.size(sizeof(std::complex<double>)));

        // Get the data
        std::vector< std::complex<double> > values(N);
        actual.receive(values, N);

        // Compare the values
        for (std::size_t i = 0; i < N; ++i) {
            expect_eq_double(expected[offset + i].real(), values[i].real(), 0.0);
            expect_eq_double(expected[offset + i].imag(), values[i].imag(), 0.0);
        }
    }

    TEST(FileSourceTest, testPlainInteger) {
        std::string filename = std::string(ORACLE_DATA_DIR) + "/oracle_file_source_integer.txt";
        const std::int64_t N = 1000;

        // Load oracle values
        std::vector<std::int64_t> oracleValues = loadValuesFromPlainFile<std::int64_t>(filename);

        // Create the task
        FileSource task(filename, FileSource::FileFormat::PlainInteger);
        Channel &out = task.getOutput(0);

        // Check if the channel was correct
        EXPECT_EQ(&task, out.getIn());

        // The task is always ready
        EXPECT_TRUE(task.isReady(0));
        EXPECT_TRUE(task.isReady(N));
        EXPECT_TRUE(task.isReady(N * 10));
        EXPECT_TRUE(task.isReady(N / 2));

        // Test the task core
        std::size_t indexOracle = 0;
        for (std::size_t i = 0; i < 2000; ++i) {
            // Computing
            EXPECT_TRUE(task.isReady(N));
            EXPECT_FALSE(task.hasFinished(N));
            task.compute(N);
            EXPECT_TRUE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Compare the result
            compareChannelWithVector(oracleValues, indexOracle, out, N);

            indexOracle = (indexOracle + N) % 100000;
        }
    }

    TEST(FileSourceTest, testPlainDouble) {
        std::string filename = std::string(ORACLE_DATA_DIR) + "/oracle_file_source_double.txt";
        const std::int64_t N = 1000;

        // Load oracle values
        std::vector<double> oracleValues = loadValuesFromPlainFile<double>(filename);

        // Create the task
        FileSource task(filename, FileSource::FileFormat::PlainDouble);
        Channel &out = task.getOutput(0);

        // Check if the channel was correct
        EXPECT_EQ(&task, out.getIn());

        // The task is always ready
        EXPECT_TRUE(task.isReady(0));
        EXPECT_TRUE(task.isReady(N));
        EXPECT_TRUE(task.isReady(N * 10));
        EXPECT_TRUE(task.isReady(N / 2));

        // Test the task core
        std::size_t indexOracle = 0;
        for (std::size_t i = 0; i < 2000; ++i) {
            // Computing
            EXPECT_TRUE(task.isReady(N));
            EXPECT_FALSE(task.hasFinished(N));
            task.compute(N);
            EXPECT_TRUE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Compare the result
            compareChannelWithVector(oracleValues, indexOracle, out, N);

            indexOracle = (indexOracle + N) % 100000;
        }
    }

    TEST(FileSourceTest, testPlainComplex) {
        std::string filename = std::string(ORACLE_DATA_DIR) + "/oracle_file_source_complex.txt";
        const std::int64_t N = 1000;

        // Load oracle values
        std::vector< std::complex<double> > oracleValues = loadValuesFromPlainFile< std::complex<double> >(filename);

        // Create the task
        FileSource task(filename, FileSource::FileFormat::PlainComplex);
        Channel &out = task.getOutput(0);

        // Check if the channel was correct
        EXPECT_EQ(&task, out.getIn());

        // The task is always ready
        EXPECT_TRUE(task.isReady(0));
        EXPECT_TRUE(task.isReady(N));
        EXPECT_TRUE(task.isReady(N * 10));
        EXPECT_TRUE(task.isReady(N / 2));

        // Test the task core
        std::size_t indexOracle = 0;
        for (std::size_t i = 0; i < 2000; ++i) {
            // Computing
            EXPECT_TRUE(task.isReady(N));
            EXPECT_FALSE(task.hasFinished(N));
            task.compute(N);
            EXPECT_TRUE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Compare the result
            compareChannelWithVector(oracleValues, indexOracle, out, N);

            indexOracle = (indexOracle + N) % 100000;
        }
    }

    TEST(FileSourceTest, testBinaryInteger) {
        std::string filename = std::string(ORACLE_DATA_DIR) + "/oracle_file_source_integer.bin";
        const std::int64_t N = 1000;

        // Load oracle values
        std::vector<std::int64_t> oracleValues = loadValuesFromBinaryFile<std::int64_t>(filename);

        // Create the task
        FileSource task(filename, FileSource::FileFormat::BinaryInteger);
        Channel &out = task.getOutput(0);

        // Check if the channel was correct
        EXPECT_EQ(&task, out.getIn());

        // The task is always ready
        EXPECT_TRUE(task.isReady(0));
        EXPECT_TRUE(task.isReady(N));
        EXPECT_TRUE(task.isReady(N * 10));
        EXPECT_TRUE(task.isReady(N / 2));

        // Test the task core
        std::size_t indexOracle = 0;
        for (std::size_t i = 0; i < 2000; ++i) {
            // Computing
            EXPECT_TRUE(task.isReady(N));
            EXPECT_FALSE(task.hasFinished(N));
            task.compute(N);
            EXPECT_TRUE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Compare the result
            compareChannelWithVector(oracleValues, indexOracle, out, N);

            indexOracle = (indexOracle + N) % 100000;
        }
    }

    TEST(FileSourceTest, testBinaryDouble) {
        std::string filename = std::string(ORACLE_DATA_DIR) + "/oracle_file_source_double.bin";
        const std::int64_t N = 1000;

        // Load oracle values
        std::vector<double> oracleValues = loadValuesFromBinaryFile<double>(filename);

        // Create the task
        FileSource task(filename, FileSource::FileFormat::BinaryDouble);
        Channel &out = task.getOutput(0);

        // Check if the channel was correct
        EXPECT_EQ(&task, out.getIn());

        // The task is always ready
        EXPECT_TRUE(task.isReady(0));
        EXPECT_TRUE(task.isReady(N));
        EXPECT_TRUE(task.isReady(N * 10));
        EXPECT_TRUE(task.isReady(N / 2));

        // Test the task core
        std::size_t indexOracle = 0;
        for (std::size_t i = 0; i < 2000; ++i) {
            // Computing
            EXPECT_TRUE(task.isReady(N));
            EXPECT_FALSE(task.hasFinished(N));
            task.compute(N);
            EXPECT_TRUE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Compare the result
            compareChannelWithVector(oracleValues, indexOracle, out, N);

            indexOracle = (indexOracle + N) % 100000;
        }
    }

    TEST(FileSourceTest, testBinaryComplex) {
        std::string filename = std::string(ORACLE_DATA_DIR) + "/oracle_file_source_complex.bin";
        const std::int64_t N = 1000;

        // Load oracle values
        std::vector< std::complex<double> > oracleValues = loadValuesFromBinaryFile< std::complex<double> >(filename);

        // Create the task
        FileSource task(filename, FileSource::FileFormat::BinaryComplex);
        Channel &out = task.getOutput(0);

        // Check if the channel was correct
        EXPECT_EQ(&task, out.getIn());

        // The task is always ready
        EXPECT_TRUE(task.isReady(0));
        EXPECT_TRUE(task.isReady(N));
        EXPECT_TRUE(task.isReady(N * 10));
        EXPECT_TRUE(task.isReady(N / 2));

        // Test the task core
        std::size_t indexOracle = 0;
        for (std::size_t i = 0; i < 2000; ++i) {
            // Computing
            EXPECT_TRUE(task.isReady(N));
            EXPECT_FALSE(task.hasFinished(N));
            task.compute(N);
            EXPECT_TRUE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Compare the result
            compareChannelWithVector(oracleValues, indexOracle, out, N);

            indexOracle = (indexOracle + N) % 100000;
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
