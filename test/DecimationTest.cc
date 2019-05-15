#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <dsps/Channel.h>
#include <dsps/Decimation.h>

#include "local/Utils.h"

namespace {
    void loadData(Channel &channel, std::uint64_t upper) {
        std::vector<double> raw;

        for (std::size_t i = 0; i < upper; ++i) {
            raw.push_back(i);
        }

        channel.send(raw);
    }

    TEST(DecimationTest, testCompute10) {
        static constexpr unsigned N = 2048;
        static constexpr unsigned D = 10;

        // Alloc the task
        Decimation task(D);
        Channel in;
        Channel &out = task.getOutput(0);

        // Connect input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());
        EXPECT_EQ(&task, out.getIn());

        std::vector<double> result;

        // Load data
        loadData(in, N * D * 100);

        do {
            EXPECT_TRUE(task.isReady(N));

            task.compute(N);

            EXPECT_TRUE(task.hasFinished(N));
        } while (task.isReady(N));

        EXPECT_FALSE(task.isReady(N));

        // Compare the result
        EXPECT_EQ(N * 100, out.size(sizeof(double)));

        std::vector<double> raw;
        out.receive(raw, N * 100);
        for (std::size_t i = 0; i < N * 100; ++i) {
            expect_eq_double(i * D, raw[i]);
        }
    }

    TEST(DecimationTest, testCompute100) {
        static constexpr unsigned N = 2048;
        static constexpr unsigned D = 100;

        // Alloc the task
        Decimation task(D);
        Channel in;
        Channel &out = task.getOutput(0);

        // Connect input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());
        EXPECT_EQ(&task, out.getIn());

        std::vector<double> result;

        // Load data
        loadData(in, N * D * 100);

        do {
            EXPECT_TRUE(task.isReady(N));

            task.compute(N);

            EXPECT_TRUE(task.hasFinished(N));
        } while (task.isReady(N));

        EXPECT_FALSE(task.isReady(N));

        // Compare the result
        EXPECT_EQ(N * 100, out.size(sizeof(double)));

        std::vector<double> raw;
        out.receive(raw, N * 100);
        for (std::size_t i = 0; i < N * 100; ++i) {
            expect_eq_double(i * D, raw[i]);
        }
    }

    TEST(DecimationTest, testCompute8) {
        static constexpr unsigned N = 2597;
        static constexpr unsigned D = 8;

        // Alloc the task
        Decimation task(D);
        Channel in;
        Channel &out = task.getOutput(0);

        // Connect input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());
        EXPECT_EQ(&task, out.getIn());

        std::vector<double> result;

        // Load data
        loadData(in, N * D * 100);

        do {
            EXPECT_TRUE(task.isReady(N));

            task.compute(N);

            EXPECT_TRUE(task.hasFinished(N));
        } while (task.isReady(N));

        EXPECT_FALSE(task.isReady(N));

        // Compare the result
        EXPECT_EQ(N * 100, out.size(sizeof(double)));

        std::vector<double> raw;
        out.receive(raw, N * 100);
        for (std::size_t i = 0; i < N * 100; ++i) {
            expect_eq_double(i * D, raw[i]);
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
