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

#include <dsps/ADC.h>
#include <dsps/Atan2.h>
#include <dsps/Channel.h>
#include <dsps/CrossSpectrum.h>
#include <dsps/Demodulation.h>
#include <dsps/Fft.h>
#include <dsps/Fir.h>
#include <dsps/FileSink.h>
#include <dsps/Hanning.h>
#include <dsps/Mean.h>
#include <dsps/NoiseGenerator.h>
#include <dsps/NormalizePsddBc.h>
#include <dsps/Splitter.h>
#include <dsps/Sum.h>
#include <dsps/Unwrap.h>
#include <dsps/Utils.h>

#include "local/Utils.h"

using ::testing::Return;

namespace {
    class MockTask: public Task {
    public:
        MockTask(const std::size_t numInput, const std::size_t numOutput)
        : Task(ChannelType::Double, numInput, ChannelType::Double, numOutput) {

        }

        MOCK_METHOD1(compute, void(const std::uint64_t));
        MOCK_CONST_METHOD1(isReady, bool(const std::uint64_t));
        MOCK_CONST_METHOD1(hasFinished, bool(const std::uint64_t));
    };

    TEST(DSPTest, testDAGLinearisationManySource) {
        // Init mock
        MockTask taskA(0, 3);
        MockTask taskB(0, 1);
        MockTask taskC(1, 1);
        MockTask taskD(1, 1);
        MockTask taskE(2, 1);
        MockTask taskF(2, 1);
        MockTask taskG(1, 1);
        MockTask taskH(2, 0);

        // Connect mock task
        Task::connect(taskA, 0, taskC, 0);
        Task::connect(taskA, 1, taskD, 0);
        Task::connect(taskA, 2, taskE, 0);
        Task::connect(taskB, 0, taskE, 1);
        Task::connect(taskC, 0, taskG, 0);
        Task::connect(taskD, 0, taskF, 0);
        Task::connect(taskE, 0, taskF, 1);
        Task::connect(taskG, 0, taskH, 0);
        Task::connect(taskF, 0, taskH, 1);

        auto result = DSP::dagLinearisation({ &taskA, &taskB });

        // Compare result
        EXPECT_EQ(static_cast<std::size_t>(8), result.size());
        auto it = result.begin();
        EXPECT_EQ(&taskA, *(it++));
        EXPECT_EQ(&taskB, *(it++));
        EXPECT_EQ(&taskC, *(it++));
        EXPECT_EQ(&taskD, *(it++));
        EXPECT_EQ(&taskE, *(it++));
        EXPECT_EQ(&taskG, *(it++));
        EXPECT_EQ(&taskF, *(it++));
        EXPECT_EQ(&taskH, *(it++));
        EXPECT_EQ(result.end(), it);
    }

    TEST(DSPTest, testDAGLinearisationManyOutput) {
        // Init mock
        MockTask taskA(0, 5);
        MockTask taskB(1, 1);
        MockTask taskC(1, 1);
        MockTask taskD(1, 2);
        MockTask taskE(1, 2);
        MockTask taskF(1, 1);
        MockTask taskG(3, 0);
        MockTask taskH(2, 0);
        MockTask taskI(2, 0);

        // Connect mock task
        Task::connect(taskA, 0, taskB, 0);
        Task::connect(taskA, 1, taskC, 0);
        Task::connect(taskA, 2, taskD, 0);
        Task::connect(taskA, 3, taskE, 0);
        Task::connect(taskA, 4, taskF, 0);
        Task::connect(taskB, 0, taskG, 0);
        Task::connect(taskC, 0, taskG, 1);
        Task::connect(taskD, 0, taskG, 2);
        Task::connect(taskD, 1, taskH, 0);
        Task::connect(taskE, 0, taskH, 1);
        Task::connect(taskE, 1, taskI, 0);
        Task::connect(taskF, 0, taskI, 1);
        auto result = DSP::dagLinearisation({ &taskA });

        // Compare result
        EXPECT_EQ(static_cast<std::size_t>(9), result.size());
        auto it = result.begin();
        EXPECT_EQ(&taskA, *(it++));
        EXPECT_EQ(&taskB, *(it++));
        EXPECT_EQ(&taskC, *(it++));
        EXPECT_EQ(&taskD, *(it++));
        EXPECT_EQ(&taskE, *(it++));
        EXPECT_EQ(&taskF, *(it++));
        EXPECT_EQ(&taskG, *(it++));
        EXPECT_EQ(&taskH, *(it++));
        EXPECT_EQ(&taskI, *(it++));
        EXPECT_EQ(result.end(), it);
    }

    TEST(DSPTest, testProcessing) {
        static constexpr unsigned N = 2048;
        static constexpr double FS = 250e6;
        static constexpr double FC = 10e6;
        static constexpr double NOISE_FLOOR_CHANNELS = 1e-14;
        static constexpr double NOISE_FLOOR_DUT = 1e-16;
        static constexpr unsigned D = 10;
        Random random;

        // Alloc all task
        NoiseGenerator<double> noiseCh1Task(random, FC, FS, NOISE_FLOOR_CHANNELS, NoiseGenerator<double>::OutputType::PHI);
        NoiseGenerator<double> noiseCh2Task(random, FC, FS, NOISE_FLOOR_CHANNELS, NoiseGenerator<double>::OutputType::PHI);
        NoiseGenerator<double> noiseDUTTask(random, FC, FS, NOISE_FLOOR_DUT, NoiseGenerator<double>::OutputType::PHI);
        Splitter<double> splitterDUTTask(2);
        Sum<double> sumCh1Task(2);
        Sum<double> sumCh2Task(2);
        ADC adcCh1Task(FC, FS, 5);
        ADC adcCh2Task(FC, FS, 5);
        Demodulation demodulationCh1Task(FC, FS, M_PI/7.0);
        Demodulation demodulationCh2Task(FC, FS, M_PI/7.0);
        Fir<double> firICH1Task(std::string(ORACLE_DATA_DIR) + "/kaiser128_40", D);
        Fir<double> firQCH1Task(std::string(ORACLE_DATA_DIR) + "/kaiser128_40", D);
        Fir<double> firICH2Task(std::string(ORACLE_DATA_DIR) + "/kaiser128_40", D);
        Fir<double> firQCH2Task(std::string(ORACLE_DATA_DIR) + "/kaiser128_40", D);
        Atan2 atan2Ch1Task;
        Atan2 atan2Ch2Task;
        Unwrap unwrapCh1Task;
        Unwrap unwrapCh2Task;
        Hanning hanningCh1Task;
        Hanning hanningCh2Task;
        Fft<double> fftCh1Task;
        Fft<double> fftCh2Task;
        CrossSpectrum crossSpectrumTask;
        NormalizePsddBc normTask(FS/D);
        // FileSink<double> normTask("/tmp/dsp_test.bin");

        // Connect noise
        Task::connect(noiseDUTTask, splitterDUTTask);
        Task::connect(splitterDUTTask, 0, sumCh1Task, 0);
        Task::connect(noiseCh1Task, 0, sumCh1Task, 1);

        Task::connect(splitterDUTTask, 1, sumCh2Task, 0);
        Task::connect(noiseCh2Task, 0, sumCh2Task, 1);

        Task::connect(sumCh1Task, 0, adcCh1Task, 0);
        Task::connect(sumCh2Task, 0, adcCh2Task, 0);

        // Connect demodulation
        Task::connect(adcCh1Task, demodulationCh1Task);
        Task::connect(adcCh2Task, demodulationCh2Task);

        // Connect FIR
        Task::connect(demodulationCh1Task, 0, firICH1Task, 0);
        Task::connect(demodulationCh1Task, 1, firQCH1Task, 0);
        Task::connect(demodulationCh2Task, 0, firICH2Task, 0);
        Task::connect(demodulationCh2Task, 1, firQCH2Task, 0);

        // Connect Atan2
        atan2Ch1Task.connectIChannel(firICH1Task);
        atan2Ch1Task.connectQChannel(firQCH1Task);
        atan2Ch2Task.connectIChannel(firICH2Task);
        atan2Ch2Task.connectQChannel(firQCH2Task);

        // Connect unwrap
        Task::connect(atan2Ch1Task, unwrapCh1Task);
        Task::connect(atan2Ch2Task, unwrapCh2Task);

        // Connect Hanning
        Task::connect(unwrapCh1Task, hanningCh1Task);
        Task::connect(unwrapCh2Task, hanningCh2Task);

        // Connect FFT
        Task::connect(hanningCh1Task, fftCh1Task);
        Task::connect(hanningCh2Task, fftCh2Task);

        // Connect cross compilation
        Task::connect(fftCh1Task, 0, crossSpectrumTask, 0);
        Task::connect(fftCh2Task, 0, crossSpectrumTask, 1);

        // Connect norm
        Task::connect(crossSpectrumTask, normTask);

        DSP::processing({ &noiseDUTTask, &noiseCh1Task, &noiseCh2Task }, { &normTask }, N);

        Channel &normCh = normTask.getOutput(0);

        EXPECT_EQ(N, normCh.size(sizeof(double)));

        // Compare the result
        std::vector<double> actualVec;
        std::ifstream fileOut(std::string(ORACLE_DATA_DIR) + "/oracle_dsp_output.bin", std::ios_base::in|std::ios_base::binary);
        auto oracleVec = extractVectorFromOracleFile<double>(fileOut, 2);

        std::vector<double> results(N);
        const std::size_t LIMIT = normCh.size(sizeof(double)) / 2;
        normCh.receive(results, N);

        for (std::size_t i = 0; i < LIMIT; ++i) {
            auto tmp = results[i];
            actualVec.push_back(tmp);
        }

        double sum = std::accumulate(actualVec.begin(), actualVec.end(), 0.0);
        double mean = sum / actualVec.size();

        double sq_sum = std::inner_product(actualVec.begin(), actualVec.end(), actualVec.begin(), 0.0);
        double stdev = std::sqrt(sq_sum / actualVec.size() - mean * mean);

        // Compare in dB
        expect_eq_double(oracleVec[oracleVec.size()-2], mean, 1);
        expect_eq_double(oracleVec[oracleVec.size()-1], stdev, 1);

        for (std::size_t i = LIMIT; i < N; ++i) {
            EXPECT_EQ(0.0, results[i]);
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
