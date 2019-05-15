#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <dsps/Channel.h>
#include <dsps/Mixer.h>

#include "local/Utils.h"

namespace {
    TEST(MixerTest, testComputeDouble) {
        static constexpr unsigned N = 2048;

        // Alloc the task
        Mixer task;
        Channel inSignal;
        Channel inNcoCos;
        Channel inNcoSin;
        Channel &outI = task.getOutput(0);
        Channel &outQ = task.getOutput(1);

        // Connect input
        task.setInput(inSignal, 0);
        task.setInput(inNcoCos, 1);
        task.setInput(inNcoSin, 2);

        // Check if the channel was correct
        EXPECT_EQ(&task, inSignal.getOut());
        EXPECT_EQ(&task, inNcoCos.getOut());
        EXPECT_EQ(&task, inNcoSin.getOut());
        EXPECT_EQ(&task, outI.getIn());
        EXPECT_EQ(&task, outQ.getIn());

        // Open the oracle file
        std::ifstream fileInSignal(std::string(ORACLE_DATA_DIR) + "/oracle_mixer_input_signal_double.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileInNcoCos(std::string(ORACLE_DATA_DIR) + "/oracle_mixer_input_nco_cos_double.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileInNcoSin(std::string(ORACLE_DATA_DIR) + "/oracle_mixer_input_nco_sin_double.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileOutI(std::string(ORACLE_DATA_DIR) + "/oracle_mixer_output_i_double.bin", std::ios_base::in|std::ios_base::binary);
        std::ifstream fileOutQ(std::string(ORACLE_DATA_DIR) + "/oracle_mixer_output_q_double.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_EQ(true, fileInSignal.good());
        ASSERT_EQ(true, fileInNcoCos.good());
        ASSERT_EQ(true, fileInNcoSin.good());
        ASSERT_EQ(true, fileOutI.good());
        ASSERT_EQ(true, fileOutQ.good());

        for (std::size_t i = 0; i < 10; ++i) {
            // If no input data
            EXPECT_EQ(false, task.isReady(N));

            // Send half data
            loadChannelFromOracleFile<double>(inSignal, fileInSignal, N/2);
            loadChannelFromOracleFile<double>(inNcoCos, fileInNcoCos, N/2);
            loadChannelFromOracleFile<double>(inNcoSin, fileInNcoSin, N/2);
            EXPECT_EQ(false, task.isReady(N));

            // Send all data
            loadChannelFromOracleFile<double>(inSignal, fileInSignal, N/2);
            loadChannelFromOracleFile<double>(inNcoCos, fileInNcoCos, N/2);
            loadChannelFromOracleFile<double>(inNcoSin, fileInNcoSin, N/2);
            EXPECT_EQ(true, task.isReady(N));
            EXPECT_EQ(false, task.hasFinished(N));

            // Computing
            task.compute(N);
            EXPECT_EQ(false, task.isReady(N));
            EXPECT_EQ(true, task.hasFinished(N));

            // Compare the result
            std::vector<double> result = extractVectorFromOracleFile<double>(fileOutI, N);
            compareChannelWithVector(result, outI, 0.0);
            result = extractVectorFromOracleFile<double>(fileOutQ, N);
            compareChannelWithVector(result, outQ, 0.0);
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
