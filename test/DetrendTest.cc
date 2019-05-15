#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <dsps/Channel.h>
#include <dsps/Detrend.h>

#include "local/Utils.h"

namespace {
    TEST(DetrendTest, testCompute) {
        static constexpr unsigned N = 2048;

        // Alloc the task
        Detrend task;
        Channel in;
        Channel &out = task.getOutput(0);

        // Connect input
        task.setInput(in, 0);

        // Check if the channel was correct
        EXPECT_EQ(&task, in.getOut());
        EXPECT_EQ(&task, out.getIn());

        std::vector<double> aCoeff = { 313.48, 87.26, 578.96, 801.14, 30.287, -66.368, -364.04, -813.36, -132.64, -994.25 };
        std::vector<double> bCoeff = { -363.29, 82.177, -885.82, 18.125, -57.73, 866.36, -755.57, -246.43, 404.50, -69.19 };

        for (std::size_t i = 0; i < 10; ++i) {
            std::vector<double> inValues(N/2);

            // If no input data
            EXPECT_FALSE(task.isReady(N));

            // Send half data
            for (std::size_t j = 0; j < N/2; ++j) {
                inValues[j] = aCoeff[i] * j + bCoeff[i];
            }
            in.send(inValues);
            EXPECT_FALSE(task.isReady(N));

            // Send all data
            inValues.clear();
            inValues.resize(N/2);
            for (std::size_t j = 0; j < N/2; ++j) {
                inValues[j] = aCoeff[i] * (j+N/2) + bCoeff[i];
            }
            in.send(inValues);

            EXPECT_TRUE(task.isReady(N));
            EXPECT_FALSE(task.hasFinished(N));

            // Computing
            task.compute(N);
            EXPECT_FALSE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            // Compare the result
            std::vector<double> outValues(N);
            out.receive(outValues, N);
            for (std::size_t j = 0; j < N; ++j) {
                expect_eq_double(0.0, outValues[j], 1e-7);
            }
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
