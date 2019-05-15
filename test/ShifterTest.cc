#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <dsps/Channel.h>
#include <dsps/Shifter.h>

#include "local/Utils.h"

namespace {
    class ShifterTest : public ::testing::Test {
    protected:
        ShifterTest()
        : m_engine(createRandomEngine())
        {

        }

        template <typename T>
        void loadChannel(Channel &channel, std::vector<T> &vector, std::size_t &vectorOffset, const std::uint64_t N) {
            EXPECT_LE(vectorOffset + N, vector.size());

            auto first = vector.begin() + vectorOffset;
            auto last = vector.begin() + vectorOffset + N;
            std::vector<T> subVector(first, last);

            channel.send(subVector);
            vectorOffset += N;
        }

        std::mt19937 m_engine;
    };

    TEST_F(ShifterTest, testComputeDouble) {
        static constexpr unsigned N = 2048;
        static constexpr unsigned SHIFT = 5;

        // Create random values
        std::vector<double> randomValues(204800);
        std::size_t indexOffset = 0;
        computeUniformFloatVector(m_engine, randomValues, -10000.0, 10000.0);
        EXPECT_EQ(static_cast<std::size_t>(204800), randomValues.size());

        // Alloc the task
        Channel in;
        Shifter<double> task(SHIFT);

        // Connect the input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());

        // If no input data
        EXPECT_FALSE(task.isReady(N));

        for (std::size_t i = 0; i < 100; ++i) {
            // Half data
            loadChannel<double>(in, randomValues, indexOffset, N/2);
            EXPECT_FALSE(task.isReady(N));

            // All data
            loadChannel<double>(in, randomValues, indexOffset, N/2);
            EXPECT_TRUE(task.isReady(N));

            // Computing
            task.compute(N);
            EXPECT_FALSE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));
        }

        // Compare the result
        Channel &out = task.getOutput(0);
        ASSERT_EQ(randomValues.size(), out.size(sizeof(double)));

        std::vector<double> values(randomValues.size());
        out.receive(values, randomValues.size());
        for (std::size_t i = 0; i < randomValues.size(); ++i) {
            expect_eq_double(randomValues[i] / std::pow(2, SHIFT), values[i], 0);
        }
    }

    TEST_F(ShifterTest, testComputeFloat) {
        static constexpr unsigned N = 2048;
        static constexpr unsigned SHIFT = 3;

        // Create random values
        std::vector<float> randomValues(204800);
        std::size_t indexOffset = 0;
        computeUniformFloatVector(m_engine, randomValues, -10000.0f, 10000.0f);
        EXPECT_EQ(static_cast<std::size_t>(204800), randomValues.size());

        // Alloc the task
        Channel in;
        Shifter<float> task(SHIFT);

        // Connect the input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());

        // If no input data
        EXPECT_FALSE(task.isReady(N));

        for (std::size_t i = 0; i < 100; ++i) {
            // Half data
            loadChannel<float>(in, randomValues, indexOffset, N/2);
            EXPECT_FALSE(task.isReady(N));

            // All data
            loadChannel<float>(in, randomValues, indexOffset, N/2);
            EXPECT_TRUE(task.isReady(N));

            // Computing
            task.compute(N);
            EXPECT_FALSE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));
        }

        // Compare the result
        Channel &out = task.getOutput(0);
        ASSERT_EQ(randomValues.size(), out.size(sizeof(float)));

        std::vector<float> values(randomValues.size());
        out.receive(values, randomValues.size());
        for (std::size_t i = 0; i < randomValues.size(); ++i) {
            expect_eq_double(randomValues[i] / std::pow(2, SHIFT), values[i], 0);
        }
    }

    TEST_F(ShifterTest, testComputeint64) {
        static constexpr unsigned N = 2048;
        static constexpr unsigned SHIFT = 6;

        // Create random values
        std::vector<std::int64_t> randomValues(204800);
        std::size_t indexOffset = 0;
        computeUniformIntegerVector(m_engine, randomValues, static_cast<std::int64_t>(-10000), static_cast<std::int64_t>(10000));
        EXPECT_EQ(static_cast<std::size_t>(204800), randomValues.size());

        // Alloc the task
        Channel in;
        Shifter<std::int64_t> task(SHIFT);

        // Connect the input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());

        // If no input data
        EXPECT_FALSE(task.isReady(N));

        for (std::size_t i = 0; i < 100; ++i) {
            // Half data
            loadChannel<std::int64_t>(in, randomValues, indexOffset, N/2);
            EXPECT_FALSE(task.isReady(N));

            // All data
            loadChannel<std::int64_t>(in, randomValues, indexOffset, N/2);
            EXPECT_TRUE(task.isReady(N));

            // Computing
            task.compute(N);
            EXPECT_FALSE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));
        }

        // Compare the result
        Channel &out = task.getOutput(0);
        ASSERT_EQ(randomValues.size(), out.size(sizeof(std::int64_t)));

        std::vector<std::int64_t> values(randomValues.size());
        out.receive(values, randomValues.size());
        for (std::size_t i = 0; i < randomValues.size(); ++i) {
            EXPECT_EQ(randomValues[i] >> SHIFT, values[i]);
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
