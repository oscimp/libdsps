#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <dsps/Channel.h>
#include <dsps/Mean.h>

#include "local/Utils.h"

namespace {
    template <typename T>
    void loadChannel(Channel &channel, T value, std::size_t N) {
        std::vector<T> values(N);
        for (std::size_t i = 0; i < N; ++i) {
            values[i] = value;
        }

        channel.send(values);
    }

    TEST(MeanTest, testComputeDouble) {
        static constexpr unsigned N = 2048;
        static constexpr unsigned M = 1e5;
        static constexpr double VALUE = 0.42;

        // Alloc the task
        Mean<double> task(M / 2.0);
        Channel in;
        Channel &out = task.getOutput(0);

        // Connect input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());
        EXPECT_EQ(&task, out.getIn());

        for (std::size_t i = 0; i < M; ++ i) {
            // No data
            EXPECT_FALSE(task.isReady(N));

            // Half data
            loadChannel<double>(in, VALUE, N/2);
            EXPECT_FALSE(task.isReady(N));

            // All data
            loadChannel<double>(in, VALUE, N/2);
            EXPECT_TRUE(task.isReady(N));

            task.compute(N);

            if (i >= M / 2 - 1) {
                EXPECT_EQ(N, out.size(sizeof(double)));
                EXPECT_TRUE(task.hasFinished(N));
            }
            else {
                EXPECT_EQ(static_cast<unsigned>(0), out.size(sizeof(double)));
                EXPECT_FALSE(task.hasFinished(N));
            }
            EXPECT_FALSE(task.isReady(N));
        }

        // Compare the result
        std::vector<double> values(N);
        out.receive(values, N);

        for (std::size_t i = 0; i < N; ++i) {
            EXPECT_EQ(VALUE, values[i]);
        }
    }

    TEST(MeanTest, testComputeComplex) {
        static constexpr unsigned N = 2048;
        static constexpr unsigned M = 1e5;
        static constexpr std::complex<double> VALUE = {0.42, 0.1337};

        // Alloc the task
        Mean< std::complex<double> > task(M / 2.0);
        Channel in;
        Channel &out = task.getOutput(0);

        // Connect input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());
        EXPECT_EQ(&task, out.getIn());

        for (std::size_t i = 0; i < M; ++ i) {
            // No data
            EXPECT_FALSE(task.isReady(N));

            // Half data
            loadChannel< std::complex<double> >(in, VALUE, N/2);
            EXPECT_FALSE(task.isReady(N));

            // All data
            loadChannel< std::complex<double> >(in, VALUE, N/2);
            EXPECT_TRUE(task.isReady(N));

            task.compute(N);

            if (i >= M / 2 - 1) {
                EXPECT_EQ(N, out.size(sizeof(std::complex<double>)));
                EXPECT_TRUE(task.hasFinished(N));
            }
            else {
                EXPECT_EQ(static_cast<unsigned>(0), out.size(sizeof(std::complex<double>)));
                EXPECT_FALSE(task.hasFinished(N));
            }
            EXPECT_FALSE(task.isReady(N));
        }

        // Compare the result
        std::vector< std::complex<double> > values(N);
        out.receive(values, N);

        for (std::size_t i = 0; i < N; ++i) {
            EXPECT_EQ(VALUE, values[i]);
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
