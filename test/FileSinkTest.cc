#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <random>

#include <dsps/Channel.h>
#include <dsps/FileSink.h>

#include "local/Utils.h"

namespace {
    class FileSinkTest : public ::testing::Test {
    protected:
        FileSinkTest()
        : m_N(2048)
        , m_engine(1337)
        , m_index(0) {
            std::uniform_real_distribution<double> dist(-1e6, +1e6);
            for (std::size_t i = 0; i < m_N * 100; ++i) {
                m_realValues.push_back(dist(m_engine));
                m_complexValues.push_back(std::complex<double>(dist(m_engine), dist(m_engine)));
            }
        }

        virtual void TearDown() {
            m_index = 0;
        }

        std::uint64_t m_N;
        std::mt19937 m_engine;
        std::vector<double> m_realValues;
        std::vector<std::complex<double>> m_complexValues;
        std::size_t m_index;
    };

    TEST_F(FileSinkTest, testComputeDouble) {
        // Alloc the task
        FileSink<double> task("/tmp/dsps_test_sink.bin");
        Channel in;

        // Connect task
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());

        for (std::size_t i = 0; i < 100; ++i) {
            // If no input data
            EXPECT_FALSE(task.isReady(m_N));

            // Send half data
            std::vector<double> inValues(m_N / 2);
            for (std::size_t j = 0; j < m_N / 2; ++j, ++m_index) {
                inValues[j] = m_realValues[m_index];
            }
            in.send(inValues);
            EXPECT_FALSE(task.isReady(m_N));

            // Send all data
            for (std::size_t j = 0; j < m_N / 2; ++j, ++m_index) {
                inValues[j] = m_realValues[m_index];
            }
            in.send(inValues);
            EXPECT_TRUE(task.isReady(m_N));

            // Computing
            if (i == 0) {
                EXPECT_FALSE(task.hasFinished(m_N));
            }
            else {
                EXPECT_TRUE(task.hasFinished(m_N));
            }
            task.compute(m_N);
            EXPECT_FALSE(task.isReady(m_N));
            EXPECT_TRUE(task.hasFinished(m_N));
        }

        // Compare the result
        std::ifstream file("/tmp/dsps_test_sink.bin", std::ios_base::in|std::ios_base::binary);
        EXPECT_TRUE(file.good());
        for (std::size_t i = 0; i < m_realValues.size(); ++i) {
            double val;
            file.read(reinterpret_cast<char*>(&val), sizeof(double));
            expect_eq_double(m_realValues[i], val, 0.0);
        }
    }

    TEST_F(FileSinkTest, testComputeComplex) {
        // Alloc the task
        FileSink< std::complex<double> > task("/tmp/dsps_test_sink.bin");
        Channel in;

        // Connect task
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());

        for (std::size_t i = 0; i < 100; ++i) {
            // If no input data
            EXPECT_FALSE(task.isReady(m_N));

            // Send half data
            std::vector< std::complex<double> > inValues(m_N / 2);
            for (std::size_t j = 0; j < m_N / 2; ++j, ++m_index) {
                inValues[j] = m_complexValues[m_index];
            }
            in.send(inValues);
            EXPECT_FALSE(task.isReady(m_N));

            // Send all data
            for (std::size_t j = 0; j < m_N / 2; ++j, ++m_index) {
                inValues[j] = m_complexValues[m_index];
            }
            in.send(inValues);
            EXPECT_TRUE(task.isReady(m_N));

            // Computing
            if (i == 0) {
                EXPECT_FALSE(task.hasFinished(m_N));
            }
            else {
                EXPECT_TRUE(task.hasFinished(m_N));
            }
            task.compute(m_N);
            EXPECT_FALSE(task.isReady(m_N));
            EXPECT_TRUE(task.hasFinished(m_N));
        }

        // Compare the result
        std::ifstream file("/tmp/dsps_test_sink.bin", std::ios_base::in|std::ios_base::binary);
        EXPECT_TRUE(file.good());
        for (std::size_t i = 0; i < m_complexValues.size(); ++i) {
            double real;
            double imag;
            file.read(reinterpret_cast<char*>(&real), sizeof(double));
            file.read(reinterpret_cast<char*>(&imag), sizeof(double));

            expect_eq_double(m_complexValues[i].real(), real, 0.0);
            expect_eq_double(m_complexValues[i].imag(), imag, 0.0);
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
