#include <complex>
#include <random>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <dsps/Queue.h>

namespace {
    TEST(QueueTest, testPushPopReal) {
        // Test the double channel
        Queue queue;
        std::vector<double> values;

        // Initial size is empty
        EXPECT_EQ(static_cast<std::size_t>(0), queue.size());

        // Write a data burst
        for (unsigned i = 0; i < 100; ++i) {
            values.push_back(i * 10.0);
        }
        queue.push(values.data(), values.size() * sizeof(double));
        EXPECT_EQ(static_cast<std::size_t>(100), queue.size() / sizeof(double));


        // Read a part of this data
        values.clear();
        values.resize(33);
        queue.pop(values.data(), 33 * sizeof(double));

        // Check size
        EXPECT_EQ(static_cast<std::size_t>(33), values.size());
        EXPECT_EQ(static_cast<std::size_t>(67), queue.size() / sizeof(double));

        // Check value
        for (unsigned i = 0; i < 33; ++i) {
            EXPECT_EQ(i * 10.0, values[i]);
        }

        // Write a data burst
        values.clear();
        for (unsigned i = 0; i < 100; ++i) {
            values.push_back((i + 100) * 10.0);
        }
        queue.push(values.data(), values.size() * sizeof(double));
        EXPECT_EQ(static_cast<std::size_t>(167), queue.size() / sizeof(double));

        // Read data
        values.clear();
        values.resize(167);
        queue.pop(values.data(), 167 * sizeof(double));

        // Check size
        EXPECT_EQ(static_cast<std::size_t>(167), values.size());
        EXPECT_EQ(static_cast<std::size_t>(0), queue.size() / sizeof(double));

        // Check value
        for (unsigned i = 0; i < 167; ++i) {
            EXPECT_EQ((i+33) * 10.0, values[i]);
        }
    }

    TEST(QueueTest, testPushPopComplex) {
        // Test the double channel
        Queue queue;
        std::vector< std::complex<double> > values;

        // Initial size is empty
        EXPECT_EQ(static_cast<std::size_t>(0), queue.size());

        // Write a data burst
        for (unsigned i = 0; i < 100; ++i) {
            values.push_back(std::complex<double>(i * 10.0, i * 0.579));
        }
        queue.push(values.data(), values.size() * sizeof(std::complex<double>));
        EXPECT_EQ(static_cast<std::size_t>(100), queue.size() / sizeof(std::complex<double>));


        // Read a part of this data
        values.clear();
        values.resize(33);
        queue.pop(values.data(), 33 * sizeof(std::complex<double>));

        // Check size
        EXPECT_EQ(static_cast<std::size_t>(33), values.size());
        EXPECT_EQ(static_cast<std::size_t>(67), queue.size() / sizeof(std::complex<double>));

        // Check value
        for (unsigned i = 0; i < 33; ++i) {
            EXPECT_EQ(i * 10.0, values[i].real());
            EXPECT_EQ(i * 0.579, values[i].imag());
        }

        // Write a data burst
        values.clear();
        for (unsigned i = 0; i < 100; ++i) {
            values.push_back(std::complex<double>((i + 100) * 10.0, (i + 100) * 0.579));
        }
        queue.push(values.data(), values.size() * sizeof(std::complex<double>));
        EXPECT_EQ(static_cast<std::size_t>(167), queue.size() / sizeof(std::complex<double>));

        // Read data
        values.clear();
        values.resize(167);
        queue.pop(values.data(), 167 * sizeof(std::complex<double>));

        // Check size
        EXPECT_EQ(static_cast<std::size_t>(167), values.size());
        EXPECT_EQ(static_cast<std::size_t>(0), queue.size() / sizeof(std::complex<double>));

        // Check value
        for (unsigned i = 0; i < 167; ++i) {
            EXPECT_EQ((i+33) * 10.0, values[i].real());
            EXPECT_EQ((i+33) * 0.579, values[i].imag());
        }
    }

    static std::mt19937 getCorrectlyInitializedEngine() {
        std::mt19937::result_type data[std::mt19937::state_size];
        std::random_device source;
        std::generate(std::begin(data), std::end(data), std::ref(source));
        std::seed_seq seeds(std::begin(data), std::end(data));
        return std::mt19937(seeds);
    }

    TEST(QueueTest, testRandomTest) {
        auto engine = getCorrectlyInitializedEngine();

        std::uint64_t currentWrite = 0;
        std::uint64_t currentRead = 0;
        std::uint64_t currentSize = 0;

        static constexpr std::size_t BufferSizeMax = 2048;
        std::array<std::uint64_t, BufferSizeMax> buffer;

        static constexpr std::size_t ChunkCountMax = 20;
        std::uniform_int_distribution<std::size_t> distChunk(1, ChunkCountMax);

        std::uniform_int_distribution<std::size_t> distSize(1, BufferSizeMax - 1);

        static constexpr std::size_t SizeThreshold = 1024;

        Queue queue;

        for (std::size_t loop = 0; loop < 100000; ++loop) {
            std::size_t chunkCount = distChunk(engine);

            for (std::size_t i = 0; i < chunkCount; ++i) {
                std::size_t size = distSize(engine);

                for (std::size_t j = 0; j < size; ++j) {
                    buffer[j] = currentWrite++;
                }

                queue.push(buffer.data(), size * sizeof(std::uint64_t));
                currentSize += size * sizeof(std::uint64_t);
            }

            ASSERT_EQ(currentSize, queue.size());

            while (queue.size() > SizeThreshold) {
                std::size_t size = distSize(engine);

                if (size * sizeof(std::uint64_t) > queue.size()) {
                    size = queue.size() / sizeof(std::uint64_t);
                }

                queue.pop(buffer.data(), size * sizeof(std::uint64_t));

                for (std::size_t j = 0; j < size; ++j) {
                    ASSERT_EQ(currentRead, buffer[j]);
                    currentRead++;
                }

                currentSize -= size * sizeof(uint64_t);
            }

            ASSERT_EQ(currentSize, queue.size());
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
